/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_toolbox1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:11:54 by varodrig          #+#    #+#             */
/*   Updated: 2024/12/23 15:29:32 by varodrig         ###   ########.fr       */
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
				exe_close(&fd[j][0]);
			}
			if (j != current_cmd)
			{
				exe_close(&fd[j][1]);
			}
		}
		else
		{
			exe_close(&fd[j][0]);
			exe_close(&fd[j][1]);
		}
		j++;
	}
}
