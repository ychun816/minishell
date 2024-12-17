/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:16:21 by varodrig          #+#    #+#             */
/*   Updated: 2024/12/15 17:26:36 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// fd : pointer to fd, if NULL it means no fd to close
//*fd : if -1, invalid or uninitialized fd
void	exe_close(int *fd)
{
	if (fd && *fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}
// only for child process
void	ft_close(t_shell *ctx)
{
	if (ctx)
	{
		exe_close(&(ctx->default_in));
		exe_close(&(ctx->default_out));
	}
}
// only for child process
// we close useless fds OR
// we close last fds when finished
void	close_fds(int pipes_nb, int (*fd)[2], int current_cmd,
		bool is_final_close)
{
	int	j;

	j = 0;
	while (j < pipes_nb)
	{
		if (!is_final_close)
		{
			if (j != current_cmd - 1)
			{
				exe_close(&fd[j][0]); // Close read ends except the one needed
			}
			if (j != current_cmd)
			{
				exe_close(&fd[j][1]); // Close write ends except the one needed
			}
		}
		else
		{
			exe_close(&fd[j][0]); // Close all read ends
			exe_close(&fd[j][1]); // Close all write ends
		}
		j++;
	}
}

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
	while (i < size)
	{
		env_arr[i] = env->env_line;
		env = env->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

void	err_execve(char *path, int err_no)
{
	int			fd_tmp;
	struct stat	stats;

	fd_tmp = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	if (err_no == 13 && stat(path, &stats) != -1)
	{
		if (S_ISDIR(stats.st_mode) == 1)
			printf("%s: %s: Is a directory\n", P_NAME, path);
		else
			printf("%s: %s: %s\n", P_NAME, path, strerror(err_no));
	}
	else
		printf("%s: %s: command not found\n", P_NAME, path);
	dup2(fd_tmp, STDOUT_FILENO);
	exe_close(&(fd_tmp));
}

// 126 : Command found but cannot be executed(due to permission issues)
// 127 : Command not found(the file does not exist or is not in the PATH)
int	ft_execution(t_shell *ctx, t_exec *temp)
{
	int		args_nb;
	char	*path;
	char	*args[ft_args_lstsize(temp->args) + 2];
	char	**env;

	args_nb = ft_args_lstsize(temp->args) + 2;
	// execve(path, comd, env);
	// char	*args[] = {"/bin/ls", "-l", "/home", NULL};
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
void	exe_err_coredump(int pid) // TODO
{
	int fd_tmp;

	fd_tmp = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("[%d]: Quit (core dumped)\n", pid);
	dup2(fd_tmp, STDOUT_FILENO);
	exe_close(&fd_tmp);
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

int	open_pipes(int pipes_nb, int (*fd)[2])
{
	int	i;
	int	j;

	i = 0;
	while (i < pipes_nb)
	{
		if (pipe(fd[i]) == -1)
		{
			j = 0;
			while (j < i)
			{
				exe_close(&fd[j][0]);
				exe_close(&fd[j][1]);
				j++;
			}
			return (-1);
		}
		i++;
	}
	return (0);
}
// only for parent process
// mode 0 : saves a copy of STDIN and STDOUT
// mode 1 : restores STDIN and STDOUT + free the copy
void	set_std(t_shell *ctx, int mode)
{
	if (!mode)
	{
		ctx->default_in = dup(STDIN_FILENO);
		ctx->default_out = dup(STDOUT_FILENO);
	}
	else
	{
		dup2(ctx->default_in, STDIN_FILENO);
		exe_close(&(ctx->default_in));
		dup2(ctx->default_out, STDOUT_FILENO);
		exe_close(&(ctx->default_out));
	}
}
// dup2 so printf can write in stderr
// open_pipes already closed fds if error
int	err_pipe(int err_no, t_shell *ctx)
{
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("%s: %s\n", PROMPT_NAME, strerror(err_no));
	set_std(ctx, 1);
	ctx->exit_code = 2;
	return (2);
}

int	err_fork(int err_no, t_shell *ctx, int fd[][2], int pipe_nb,
		int fork_success)
{
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("%s: %s\n", PROMPT_NAME, strerror(err_no));
	close_all(pipe_nb, fd);
	set_std(ctx, 1);
	exe_wait_all(fork_success, ctx);
	ctx->exit_code = 2;
	return (2);
}

// only for parent process
void	close_all(int pipe_nb, int (*fd)[2])
{
	int	i;

	i = 0;
	while (i < pipe_nb)
	{
		exe_close(&fd[i][0]);
		exe_close(&fd[i][1]);
		i++;
	}
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

// stores copy of STDOUT
// redirects STDOUT to STDERR
// printf can now write in STDERR
// restores STDOUT
// free the copy
void	err_open(int err_no, char *file)
{
	int	temp_fd;

	temp_fd = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("%s: %s: %s\n", PROMPT_NAME, file, strerror(err_no));
	dup2(temp_fd, STDOUT_FILENO);
	exe_close(&(temp_fd));
}

void	redirs_type(t_filename *file)
{
    int fd = -1;
    int target_fd = STDOUT_FILENO;
    int flags = O_WRONLY | O_CREAT | O_TRUNC;

    fprintf(stderr, "entered redirs_type\n");

    // Déterminer les flags et le descripteur cible
    if (file->type == INFILE || file->type == NON_HEREDOC)
    {
        target_fd = STDIN_FILENO;
        flags = O_RDONLY;
    }
    else if (file->type == APPEND)
    {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    }

    // Ouvrir le fichier
    fd = open(file->path, flags, 0644);
    if (fd == -1)
    {
        err_open(errno, file->path);
        return;
    }

    // Rediriger
    if (dup2(fd, target_fd) == -1)
    {
        err_open(errno, file->path);
        close(fd);
        return;
    }

    // Fermer explicitement le descripteur de fichier
    close(fd);

    fprintf(stderr, "redirs_type successful\n");
}

// goes through redirs list
// fd_in and fd_out = -1 if open failed so no need to close
int	exec_redirs(t_exec *exec)
{
	t_filename	*redirs;

	redirs = exec->redirs;
	while (redirs)
	{
		redirs_type(redirs);
		if (exec->fd_in == -1 || exec->fd_out == -1)
			return (1);
		redirs = redirs->next;
	}
	return (0);
}

// goes through exec list
int	err_redirs(t_exec *exec)
{
	while (exec)
	{
		if (exec_redirs(exec) == 1)
			return (1);
		exec = exec->next;
	}
	return (0);
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
