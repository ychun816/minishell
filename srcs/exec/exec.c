#include "minishell.h"

int	ft_args_lstsize(t_arg *args)
{
	t_arg	*curr;
	int		count;

	count = 0;
	curr = args;
	while (curr)
	{
		curr = curr->next;
		count++;
	}
	return (count);
}

void	exec_args_create(t_exec *temp, int args_nb, char *args[args_nb])
{
	t_arg	*curr;
	int		i;

	i = 0;
	args[i] = temp->cmd;
	curr = temp->args;
	i++;
	while (curr)
	{
		args[i] = curr->value;
		curr = curr->next;
		i++;
	}
	args[i] = NULL;
}

int	ft_char_count(char *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i++] == c)
			count++;
	}
	return (count);
}

void	ft_free_all(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

// access() returns 0 if OK
char	*find_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*path;
	char	*exec;
	t_env	*curr;
	int		i;

	curr = env;
	while (curr && ft_strncmp("PATH=", curr->env_line, 5) != 0)
		curr = curr->next;
	if (!curr)
		return (ft_strdup(cmd));
	paths = ft_split(curr->value, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		exec = ft_strjoin(path, cmd);
		free(path);
		if (!access(exec, X_OK | F_OK))
		{
			ft_free_all(paths);
			return (exec);
		}
		free(exec);
		i++;
	}
	ft_free_all(paths);
	return (NULL);
}

int	ft_env_lstsize(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**env_format(t_env *env)
{
	char	**env_arr;
	int		size;
	int		i;

	i = 0;
	size = ft_env_lstsize(env);
	env_arr = (char **)malloc(sizeof(char *) * (size + 1));
	if (!env)
		return (NULL);
	while (i < size && env)
	{
		env_arr[i] = env->env_line;
		env = env->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

// 126 : Command found but cannot be executed(due to permission issues)
// 127 : Command not found(the file does not exist or is not in the PATH)
// execve(path, comd, env);
// char	*args[] = {"/bin/ls", "-l", "/home", NULL};
int	ft_execution(t_shell *ctx, t_exec *temp)
{
	int		args_nb;
	char	*path;
	char	*args[ft_args_lstsize(temp->args) + 2];
	char	**env;

	args_nb = ft_args_lstsize(temp->args) + 2;
	exec_args_create(temp, args_nb, args);
	env = env_format(ctx->env);
	if (!env)
		return (4);
	if (execve(temp->cmd, args, env) == -1)
	{
		path = find_path(temp->cmd, ctx->env);
		if (!path)
		{
			err_execve(temp->cmd, errno);
			free(env);
			return (4);
		}
		if (execve(path, args, env) == -1)
		{
			err_execve(path, errno);
			free(path);
			free(env);
			return (-2);
		}
		free(path);
	}
	free(env);
	return (0);
}

// we can exit here because we are in a child process
void	child_process(t_shell *ctx, int (*fd)[2], int i, t_exec *temp)
{
	int	exit_code;

	exit_code = 0;
	signal(SIGQUIT, SIG_DFL);
	if (ctx->exec_count > 1)
	{
		if (i > 0)
		{
			dup2(fd[i - 1][0], STDIN_FILENO);
			exe_close(&fd[i - 1][0]);
		}
		if (i < ctx->exec_count - 1)
		{
			dup2(fd[i][1], STDOUT_FILENO);
			exe_close(&fd[i][1]);
		}
		close_fds(ctx->exec_count - 1, fd, i, false);
	}
	ft_close(ctx);
	if (err_redirs(temp))
	{
		free_all_shell(ctx);
		ctx->exit_code = 1;
		exit(ctx->exit_code);
	}
	if (check_is_builtin(temp->cmd))
	{
		exit_code = exec_builtin(ctx, temp->cmd, temp->args);
		free_all_shell(ctx);
		exit(exit_code);
	}
	exit_code = ft_execution(ctx, temp);
	free_all_shell(ctx);
	if (exit_code == -2)
		exit(126);
	exit(127);
}

void	exe_wait_all(int pid_count, t_shell *ctx) // TODO
{
	int status;
	int i;

	i = 0;
	while (i < pid_count)
	{
		if (waitpid(ctx->pids[i], &(status), 0))
		{
			if (WIFEXITED(status))
			{
				g_signal.signal_code = 0;
				ctx->exit_code = WEXITSTATUS(status);
			}
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
					exe_err_coredump(ctx->pids[i]);
				g_signal.signal_code = SIGNAL_OFFSET + WTERMSIG(status);
			}
		}
		i++;
	}
	unlink_all(ctx);
}

int	exec_parent(t_shell *ctx)
{
	t_exec	*temp;
	int		fd[ctx->exec_count][2];
	pid_t	pid;

	temp = ctx->exec;
	if (open_pipes(ctx->exec_count - 1, fd) == -1)
		return (err_pipe(errno, ctx));
	while (temp)
	{
		signal(SIGINT, sig_exec);
		pid = fork();
		if (pid == -1)
			return (err_fork(errno, ctx, fd, ctx->pid_count - 1,
					ctx->pid_count));
		else if (pid == 0)
			child_process(ctx, fd, ctx->pid_count, temp);
		ctx->pids[ctx->pid_count] = pid;
		ctx->pid_count++;
		temp = temp->next;
	}
	close_all(ctx->exec_count - 1, fd);
	set_std(ctx, 1);
	exe_wait_all(ctx->pid_count, ctx);
	return (0);
}

void	unlink_all(t_shell *ctx)
{
	t_exec		*exec;
	t_filename	*redir;

	exec = ctx->exec;
	while (exec)
	{
		redir = exec->redirs;
		while (redir)
		{
			if (redir->type == NON_HEREDOC)
				unlink(redir->path);
			redir = redir->next;
		}
		exec = exec->next;
	}
}

// store STDIN & STDOUT
// 1 command and its a builtin (no loop)
// if errors in redirections, exit
// unlink (close) all the N_HEREDOC(unlink in the else also)
int	exec(t_shell *ctx)
{
	t_exec	*temp;

	if (ctx->exec_count == 0)
		return (0);
	set_std(ctx, 0);
	temp = ctx->exec;
	if (ctx->exec_count == 1 && check_is_builtin(temp->cmd))
	{
		if (err_redirs(temp))
		{
			ctx->exit_code = 1;
			return (ctx->exit_code);
		}
		if (check_is_builtin(temp->cmd) == 2)
			ft_putstr_fd("exit\n", STDERR_FILENO);
		unlink_all(ctx);
		ctx->exit_code = exec_builtin(ctx, temp->cmd, temp->args);
		set_std(ctx, 1);
		return (ctx->exit_code);
	}
	exec_parent(ctx);
	return (0);
}
