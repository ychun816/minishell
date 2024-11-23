/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_to_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:05:40 by yilin             #+#    #+#             */
/*   Updated: 2024/11/23 20:39:44 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**	init_build();
 */
t_exec	*init_build(void)
{
	t_exec	*new;

	new = malloc(sizeof(t_exec));
	if (!new)
		return (NULL);
	new->cmd = NULL;
	new->args = NULL;
	new->redirs = NULL;
	new->next = NULL;
	new->fd_in = STDIN_FILENO;
	new->fd_out = STDOUT_FILENO;
	return (new);
}

/** BUILD_FOR_EXEC
 * 
 * Builds a `t_exec` structure from the linked list of tokens.
 * 
 * (1) Initialize the `t_exec` structure; If fail -> return NULL
 * (2) Loop thru token list
 * -1 Handle PIPE ->  Recursively creating a new `t_exec` structure for the next command.
 * -2 Handle COMMAND -> Storing its value in the `cmd` field.
 * -3 Handle redirection tokens
 * -4 Handle argument tokens
 * 
 * @note
 * exec->next = builder(token->next);
 * 
 * @return constructed `t_exec` structure
 * 
 */
t_exec *build_for_exec(t_token *token)
{
	t_exec	*exec;
	
	exec = init_build();
	if (!exec)
		return (NULL);
	while (token != NULL)
	{
		if (token->type == PIPE) //PIPE
		{
			exec->next = build_for_exec(token->next); //recursive
			break ;
		}
		else if (token->type == COMMAND) //COMMAND
		{
			exec->cmd = ft_strdup(token->value);
			if (!exec->cmd)
				return (NULL);
		}
		bd_handle_redirs(exec, token);
		bd_handle_args(exec, token);
		token = token->next;
	}
	return (exec);
}