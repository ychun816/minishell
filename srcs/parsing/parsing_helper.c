/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:48:18 by yilin             #+#    #+#             */
/*   Updated: 2024/12/07 17:47:54 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**FOR PASING- GET ENV
 * Search for an environment variable by its name (path) in a linked list of environment variables (env).
 * 
*/
t_env	*get_env(char *pathname, t_env *env)
{
	//chek if either is NULL
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


/** PARSE- EXPAND ENV
 * Iterates through all tokens in a list and applies ps_handle_env to process $VAR expansions.
 * 
 * 
 * Loops through a list of tokens and expands environment variables in tokens of type STRING or DOUBLEQUOTE.
 * It calls (ps_handle_env) to handle the expansion for each token that requires it.
 * 
 * @return Tokens updated with $VAR expansions.
 * 
 */
int	prs_expand_env(t_token *token)
{
	while (token != NULL)
	{
		if (token->type == STR || token->type == DBL_QUOTE)
		{
			prs_handle_envvar_expansion(token);
		}
		token = token->next;
	}
	return (SUCCESS);	
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