/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:11:31 by varodrig          #+#    #+#             */
/*   Updated: 2024/12/23 15:33:55 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
