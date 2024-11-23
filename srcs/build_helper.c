/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 19:15:24 by yilin             #+#    #+#             */
/*   Updated: 2024/11/23 20:33:32 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**build_listsize */
int	build_listsize(t_exec *exec)
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

/**build_free all*/
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