/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:11:57 by varodrig          #+#    #+#             */
/*   Updated: 2025/01/06 17:44:41 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void	redirs_type(t_exec *exec, t_filename *file)
{
	int	fd;
	int	target_fd;
	int	flags;

	if (file->type == INFILE || file->type == NON_HEREDOC)
	{
		if (exec->fd_in != STDIN_FILENO)
			close (exec->fd_in);
		exec->fd_in = open(file->path, O_RDONLY);
		if (exec->fd_in == -1)
			err_open(errno, file->path);
		dup2(exec->fd_in, STDIN_FILENO);
		exe_close(&exec->fd_in);
	}
	else
	{
		if (exec->fd_out != STDOUT_FILENO)
			close(exec->fd_out);
		if (file->type == OUTFILE)
			exec->fd_out = open(file->path,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (file->type == APPEND)
			exec->fd_out
				= open(file->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (exec->fd_out == -1)
			err_open(errno, file->path);
		dup2(exec->fd_out, STDOUT_FILENO);
		exe_close(&exec->fd_out);
	}
}*/
//link between redir file and cmd done with dup2
int	redirs_type(t_filename *file)
{
	int	fd;
	int	target_fd;
	int	flags;

	fd = -1;
	target_fd = STDOUT_FILENO;
	flags = O_WRONLY | O_CREAT | O_TRUNC;
	if (file->type == INFILE || file->type == NON_HEREDOC)
	{
		target_fd = STDIN_FILENO;
		flags = O_RDONLY;
	}
	else if (file->type == APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(file->path, flags, 0644);
	if (fd == -1)
		return (err_open(errno, file->path), 1);
	if (dup2(fd, target_fd) == -1)
	{
		err_open(errno, file->path);
		exe_close(&fd);
		return (1);
	}
	exe_close(&fd);
	return (0);
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
