/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 19:15:24 by yilin             #+#    #+#             */
/*   Updated: 2025/01/07 11:37:17 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_build_lstsize(t_exec *exec)
{
	int	len;

	len = 0;
	while (exec)
	{
		exec = exec->next;
		len++;
	}
	return (len);
}

void	build_free_all(t_exec *exec)
{
	t_exec	*current;

	while (exec != NULL)
	{
		current = exec;
		if (exec->cmd)
			free(exec->cmd);
		if (exec->args)
			arg_free(exec->args);
		if (exec->redirs)
			filename_free(exec->redirs);
		exec = exec->next;
		free(current);
	}
}
