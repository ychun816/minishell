/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:38:02 by yilin             #+#    #+#             */
/*   Updated: 2025/01/07 17:56:39 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** CHECK_LINE_EMPTY */
int	check_line_empty(char *line)
{
	if (line[0] == '\0')
		return (1);
	return (0);
}

/** FREE AFTER PROCESS
 * (1) free used token
 * (2) free what's inside shell: free build, pid
 *
 * @note
 * The pids array in content prob stores process IDs of child processes
   that the shell has spawned
 * =>  Set to NULL at the end
 * -> Preventing Dangling Pointers
 * -> Safety in Future Operations
 * -> Preventing Unintended Access After Freeing
 */
void	free_after_process(t_shell *content, t_token *token)
{
	if (token)
		token_free(token);
	if (content)
	{
		if (content->exec)
		{
			build_free_all(content->exec);
			content->exec = NULL;
		}
		if (content->pids)
		{
			free(content->pids);
			content->pids = NULL;
		}
	}
}

/* FREE ALL SHELL */
void	free_all_shell(t_shell *content)
{
	if (content)
	{
		if (content->exec)
			build_free_all(content->exec);
		if (content->env)
			env_free(content->env);
		if (content->pids)
			free(content->pids);
		free(content);
	}
}
