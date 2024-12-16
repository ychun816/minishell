/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:04:39 by yilin             #+#    #+#             */
/*   Updated: 2024/12/16 16:16:25 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** CHECK QUOTE VALID
 * validation step -> prevent errors caused by unclosed or invalid quotes before running the more complex functions.
 * 
 * @note *EXTRA CONDITION after ft_quotes_len(), 
 * 			if (str[i] == '\0')
 * 				return (FAILURE);
 *  -> to detect unmatched or missing closing quotes.
 *  
 * If the function encounters the end of the string ('\0') after processing a quote, but without finding a matching closing quote, it means that the quotes are improperly terminated. 
 * This can happen if:

    An opening quote exists (' or "), but there is no corresponding closing quote at the end of the string.
	The string is incomplete or malformed.
	
 */
int	prs_check_quotes_valid(t_token *token)
{
	int i;
	char *str;

	while (token != NULL)
	{
		if (token->type == STR)
		{
			i = 0;
			str = token->value;
			while (str[i])
			{
				if (str[i] == '\'' || str[i] == '\"') // If a quote is found, process it
					i += ft_quotes_len(&(str[i]), str[i]);//incremet i to the end of unquote
				if (str[i] == '\0')
					return (FAILURE);
				i++;
			}
		}
		token = token->next;
	}
	return (SUCCESS);
}
/** prs_get_quoted_str
 * (1) Calculate the length of the quoted string (excluding the closing quote).
 * (2)  Default to DOUBLEQUOTE type
 * (3) Check if inside the quote has context
 *    [IF] has context  -> Create a token starting after the opening quote (`str + 1`) of the given length
 * 	  [ELSE] NO context -> Inside the quotes, create an empty string token.
 * 
 * @note
 * len = ft_quotes_len(input_str, c) - 1; => skip closing quote (ft_quotes_len)
 * new_token = creat_token(input_str + 1, len, type, content); => skip opening quote (input_str + 1)
 * new_token = creat_token("\0", 1, STR, content):
 * "\0": represent an empty string (null-terminated), a string with no content.
 * STR: This type indicates that it is just a regular string (not quoted or other types).
 * 
 */
t_token	*prs_get_quoted_str(char *input_str, char c, t_shell *content)
{
	int	len;
	t_token	*new_token;
	t_token_type	type;
	
	if (!input_str || ft_quotes_len(input_str, c) <= 1) // ADDED to Handle unmatched or empty quotes
        return (NULL);
	len = ft_quotes_len(input_str, c) - 1; 
	new_token = NULL;
	type = DBL_QUOTE;
	if (c == '\'')
		type = SGL_QUOTE;	
	if (len > 0)
		new_token = token_create(input_str + 1, len, type, content);
	else
		new_token = token_create("\0", 1, STR,  content);
	return (new_token);	
}

/** ft_rogue_len*/
int	ft_rogue_len(char	*str)
{
	int	len;
	
	len = 0;
	while (str[len])
	{
		if (str[len] == '\'' || str[len] == '\"')
			break ;
		len++;
	}
	return (len);
}

/** prs_quotes_to_tokens
 * Parses a string into tokens by:
 * - Identifying quoted and unquoted parts of the input string.
 * - Creating tokens for each segment (quoted or unquoted).
 * - Returning a linked list of tokens representing the parsed segments.
 * 
 * (1) Loop input_str
 * 		[IF] the current character is a single or double quote
 * 		-> Extracts the content inside the quotes + creates a token for it
 * 		-> Skip the closing quote by moving the index `i` forward by the length of the processed string + 1
 * 		[ELSE] not quotes
 * 		-> Create a raw token for the unquoted part
 * 		-> Move `i` forward by the length of the raw segment minus 1
 *   	 (since the `i++` at the end of the loop increments it further)
 * (2) If token creation failed -> return NULL
 * (3) Add the newly created token to the list of tokens
 * @return result (newly created token)
 */
t_token	*prs_quotes_to_tokens(char *input_str, t_shell *content)
{
	int	i;
	t_token	*token;
	t_token	*new_token;

	i = 0;
	token = NULL;
	new_token = NULL;
	while (input_str[i])
	{
		while (input_str[i] == ' ') //ADD to skip space??
			i++;
		if (input_str[i] == '\'' || input_str[i] == '\"')
		{
			new_token =  prs_get_quoted_str(&(input_str[i]), input_str[i], content);
			if (!new_token) //Added for protection
					return (NULL);
			i += ft_strlen(new_token->value) + 1;
		}
		else
		{
			new_token = token_create(&(input_str[i]), ft_rogue_len(&(input_str[i])), STR, content);
			if (!new_token) //Added for protection
				return (NULL);
			i += ft_strlen(new_token->value) - 1;
		}
		// if (!new_token)
		// 	return (NULL);
		token_add_back(&token, new_token);
		i++;
	}
	return (token);
}

/**handle quotes n expand env
 * processes tokens in a linked list of tokens (t_token) by handling:
 * 
 * (1) Iterate through all tokens in the linked list (= input string)
 * (2) Process tokens of type STRING:
 * 	   -1 Parse quotes within the token's value and return a list of tokens
 * 	   -2 Expand environment variables within the parsed tokens 
 */
int	prs_handle_quotes_n_expand_env(t_token *token)
{
	t_token	*input_str;

	while (token != NULL)
	{
		if (token->type == STR)
		{
			input_str = prs_quotes_to_tokens(token->value, token->content);
			prs_expand_env(input_str);
			if (token->value)
				free(token->value);
			token->value = prs_tokens_combine(input_str);
			token_free(input_str);
		}
		token = token->next;
	}
	return (SUCCESS);
}