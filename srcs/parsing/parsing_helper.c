/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:48:18 by yilin             #+#    #+#             */
/*   Updated: 2024/12/23 16:06:25 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**FOR PASING- GET ENV
 * Search for an environment variable by its name (path) in a linked list of environment variables (env). 
*/
t_env	*get_env(char *pathname, t_env *env)
{
	if (pathname == NULL || env == NULL)
		return (NULL);
	while (env != NULL)
	{
		if (ft_strcmp(pathname, env->id) == 0)
			return (env);		
		env = env->next;
	}
	return (NULL);
}



/** PRS TOKENS COMBINE 
 * 
 * (1) Duplicate an empty string ("")(Initialize `value` as an empty string to start the concatenation process.)
 * (2) Loop thru token list
 * - If the current token has a non-NULL value, concatenate it,
 * -> Append the token's value to `value` using `prs_strjoin`
 * 
*/
char	*prs_tokens_combine(t_token *token)
{
	char	*result;

	result = ft_strdup("");
	while (token != NULL)
	{
		if (token->value != NULL)
		{
			result = prs_strjoin(result, token->value);
			if (!result)
				break ;
		}
		token = token->next;
	}
	return (result);
}

/** CHECK ALL NODES NULL
 * Checks if all nodes in the linked list have NULL values, 
 * 
 * @return 1 if found NULL value
 * @return 0 if found non-NULL value.
 * 
 */
int	prs_check_allnodes_null(t_token *token)
{
	while (token)
	{
		if (token->value)
			return (SUCCESS);
		token = token->next;
	}
	return (FAILURE_VOID);
}

// UNLINK ERROR
//  * Iterates through a linked list of t_token nodes,
//  * When it encounters a node with a specific type (NON_HEREDOC), 
//  * It unlinks (deletes) a file whose path is stored
//  * 		in the value field of the next node. 
//  * 
//  * @note 
//  * unlink(): 
//  * - Delete temporary files associated with here-documents.
//  * - Ensures that these temporary files are properly removed.
void	prs_unlink_error(t_token *token)
{	
	while (token)
	{
		if (token->type == NON_HEREDOC)
			unlink(token->next->value);
		token = token->next;
	}
}
