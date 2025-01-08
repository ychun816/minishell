/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:12:05 by varodrig          #+#    #+#             */
/*   Updated: 2025/01/08 15:48:16 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	err_fork(int err_no, t_shell *ctx, int fd[][2])
{
	int	pipe_nb;
	int	fork_success;

	pipe_nb = ctx->pid_count - 1;
	fork_success = ctx->pid_count;
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("%s: %s\n", PROMPT_NAME, strerror(err_no));
	close_all(pipe_nb, fd);
	set_std(ctx, 1);
	wait_children(fork_success, ctx);
	ctx->exit_code = 2;
	return (2);
}

//TODO
void	exe_err_coredump(int pid)
{
	int	fd_tmp;

	fd_tmp = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("[%d]: Quit (core dumped)\n", pid);
	dup2(fd_tmp, STDOUT_FILENO);
	exe_close(&fd_tmp);
}

// TODO
void	err_execve(char *path, int err_no)
{
	int			fd_tmp;
	struct stat	stats;

	fd_tmp = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	if (err_no == 13 && stat(path, &stats) != -1)
	{
		if (S_ISDIR(stats.st_mode) == 1)
			printf("%s: %s: Is a directory\n", PROMPT_NAME, path);
		else
			printf("%s: %s: %s\n", PROMPT_NAME, path, strerror(err_no));
	}
	else
		printf("%s: %s: command not found\n", PROMPT_NAME, path);
	dup2(fd_tmp, STDOUT_FILENO);
	exe_close(&(fd_tmp));
}
