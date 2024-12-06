/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:02 by yilin             #+#    #+#             */
/*   Updated: 2024/12/06 17:33:20 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/** LEX GET TOKEN TYPE
 * - Determines the token type based on the first character of the string.
 * - It checks for special characters like '<', '>', and '|' and assigns the corresponding token type
 * @return 
 * HEREDOC if "<<"
 * APPEND if ">>"
 * Otherwise: INFILE, OUTFILE, PIPE, or STR.
 */
t_token_type lex_get_token_meta_type(char *str)
{
	if (str[0] == '>')
	{
		if (str[1] == '>')
			return (APPEND);
		else
			return (OUTFILE);
	}
	else if (str[0] == '<')
	{
		if (str[1] == '<')
			return (HEREDOC);
		else
			return (INFILE);
	}
	else if (str[0] == '|')
		return (PIPE);
	return (STR);
}

/** LEX FT_1TOKEN_LEN
 * - Count len of a token based on its type
 * - This len helps to identify how many characters to include in a token.
 * @param str: A pointer to the start of the token in the input string.
 * @param type: The type of the token (e.g., HEREDOC, APPEND, INFILE, OUTFILE, PIPE, STRING).
 * 
 * - INFILE (>), OUTFILE (<), or PIPE (|) => len = 1
 * - HEREDOC (<<) or APPEND (>>) => len = 2
 * - STR => ft_token_str_len()
 * 
 */
int	lex_ft_1tokenlen(char *str, t_token_type type)
{
	int	len;

	len = 0;
	if (type == INFILE || type == OUTFILE || type == PIPE)
		len = 1;
	else if (type == HEREDOC || type == APPEND)
		len = 2;
	else if (type == STR)
		len = ft_token_str_len(str);
	return (len);	
}

/** FT_TOKEN_STRLEN
 * - Calculates the length of a "string" token,
 *   which is any sequence of characters, not a meta character and does not contain spaces or tabs.
 * - If meet quoted substrings -> skip over them.
 * - Stop 
 */
int ft_token_str_len(char *str)
{
	int	len;
	int	total_len;

	len = 0;
	total_len = ft_strlen(str); //the whole string len
	while (len < total_len)
	{
		//single quote
		if (str[len] == '\'')
			len += ft_quotes_len(&(str[len]), '\''); //str
		//double quote
		else if (str[len] == '\"')
			len += ft_quotes_len(&(str[len]), '\"'); //str
		//break if meet a meta character
		else if (check_meta_char(str[len]) || str[len] == '\t' || str[len] == ' ')
			break ;
		len++;
	}
	return (len);
}

/**FT_QUOTE_LEN
 * - Calculates the length of a quoted substring.
 * - It starts after the opening quote and continues until it finds the matching closing quote.
*/
int ft_quotes_len(char *str, char sd_quote) //single, double quote
{
	int	len;

	len = 1;
	while (str[len] && str[len] != sd_quote)
		len++;
	return (len);
} 

/** CHECK META CHAR
 * checks if the character `c` is a meta character ( |, <, >, single quotes, double quotes).
 * @return a non-zero value if `c` is a meta character -> to identify boundaries in tokens.
 */
int check_meta_char(char c)
{
	//Stop at the closing single quote ('
	int	found;

	found = 0;
	if (c == '|')
		found = 1;
	else if (c == '<')
		found = 2;
	else if (c == '>')
		found = 3;
	else if (c == '\"')
		found = 4;
	else if (c == '\'')
		found = 5;
	return (found);
}
