/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_toolbox2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:11:51 by varodrig          #+#    #+#             */
/*   Updated: 2024/12/23 15:30:48 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// TODO
void	exe_wait_all(int pid_count, t_shell *ctx)
{
	int	status;
	int	i;

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
