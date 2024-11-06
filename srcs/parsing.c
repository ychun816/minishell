/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:02:54 by yilin             #+#    #+#             */
/*   Updated: 2024/11/06 19:53:59 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** QUOTES
 *
 * @note *EXTRA CONDITION after ft_quotes_len(), 
 * 			if (str[i] == '\0')
 * 				return (FAILURE);
 *  -> to detect unmatched or missing closing quotes.
 *  
 * If the function encounters the end of the string ('\0') after processing a quote, but without finding a matching closing quote, it means that the quotes are improperly terminated. This can happen if:

    An opening quote exists (' or "), but there is no corresponding closing quote at the end of the string.
	The string is incomplete or malformed.
	
 */
int	prs_handle_quotes(t_token *token)
{
	int i;
	char *str;

	while (token)
	{
		if (token->type == STR)
		{
			i = 0;
			while (str[i])
			{
				if (str[i] == '\'' || str[i] == '\"') // If a quote is found, process it
					i += ft_quotes_len(&(str[i]), str[i]);//incremet i to the end of unquote
				i++;
			}
			if (str[i] == '\0')
				return (FAILURE);
		}
		token = token->next;
	}
	return (SUCCESS);
}

/** REDIR
 * 
 * 
 */
int	prs_handle_redir(t_token *token)
{
	
}

/** CMD
 *
 *  
 */
int	prs_handle_cmd(t_token *token)
{
	
}

/** HEREDOC 
 *
 *  
*/
int	prs_handle_heredoc(t_token *token)
{
	
}


/** PARSING */
int	parsing(t_token **token)
{
	int	return_code;

	return_code = 0;
	if (prs_handle_quotes(*token) != 0)
		return_code = FAILURE;
	else if (prs_expand_n_quotes(*token) != 0)
		return_code = FAILURE;
	else if (prs_remove_null(token) != 0)
		return_code = FAILURE;
	else if (prs_check_all_null(*token) != 0)
		return_code = FAILURE_VOID;
	else if (prs_handle_redir(*token) != 0)
		return_code = FAILURE;
	else if (prs_handle_cmd(*token) != 0)
		return_code = FAILURE;
	else if (prs_handle_heredoc(*token) != 0)
	{
		prs_unlink_err(*token);
		return_code = FAILURE_VOID;
	}
	return (return_code);
}