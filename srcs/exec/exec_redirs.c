/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:11:57 by varodrig          #+#    #+#             */
/*   Updated: 2024/12/23 15:35:39 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirs_type(t_exec *exec, t_filename *file)
{
	if (file->type == INFILE || file->type == NON_HEREDOC)
	{
		exec->fd_in = open(file->path, O_RDONLY);
		if (exec->fd_in == -1)
			err_open(errno, file->path);
		dup2(exec->fd_in, STDIN_FILENO);
		exe_close(exec->fd_in);
	}
	else
	{
		if (file->type == OUTFILE)
			exec->fd_out = open(file->path,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (file->type == APPEND)
			exec->fd_out
				= open(file->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (exec->fd_out == -1)
			err_open(errno, file->path);
		dup2(exec->fd_out, STDOUT_FILENO);
		exe_close(exec->fd_out);
	}
}

// goes through redirs list
// fd_in and fd_out = -1 if open failed so no need to close
int	exec_redirs(t_exec *exec)
{
	t_filename	*redirs;

	redirs = exec->redirs;
	while (redirs)
	{
		redirs_type(exec, redirs);
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
