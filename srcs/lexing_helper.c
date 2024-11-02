/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:02 by yilin             #+#    #+#             */
/*   Updated: 2024/11/02 19:50:22 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**FT_TOKEN_STRLEN
 * - Calculates the length of a "string" token,
 *   which is any sequence of characters, not a meta character and does not contain spaces or tabs.
 * - If meet quoted substrings -> skip over them.
 * 
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
			len += ft_quotelen(str, '\'');
		//double quote
		else if (str[len] == '\"')
			len += ft_quotelen(str, '\"');
		//break if meet a meta character
		else if (check_meta_char(str[len]) != 0 || str[len] == '\t' || str[len] == ' ')
			break ;
		len++;
	}
	return (len);
}

/**FT_QUOTELEN
 * - Calculates the length of a quoted substring.
 * - It starts after the opening quote and continues until it finds the matching closing quote.
*/
int ft_quotelen(char *str, char sd_quote) //single, double quote
{
	int	len;

	len = 1;
	while (str[len] && str[len] != sd_quote)
		len++;
	return (len);
} 

/** GET TOKEN TYPE
 * - Determines the token type based on the first character of the string.
 * - It checks for special characters like '<', '>', and '|' and assigns the corresponding token type
 * @return 
 * HEREDOC if "<<"
 * APPEND if ">>"
 * Otherwise: INFILE, OUTFILE, PIPE, or STR.
 * 
 */
t_token_type get_token_type(char *str)
{
	// > ->find next > (>>)
	if (str[0] == '>')
	{
		if (str[1] == '>')
			return (APPEND);
		else
			return (OUTFILE);
	}
	// < ->find next < (<<)
	else if (str[0] == '<')
	{
		if (str[1] == '<')
			return (HEREDOC);
		else
			return (INFILE);
	}
	// |
	else if (str[0] == '|')
		return (PIPE);
	return (STR);
}


/** CHECK META CHAR
 * checks if the character `c` is a meta character ( |, <, >, single quotes, double quotes).
 * @return a non-zero value if `c` is a meta character -> to identify boundaries in tokens.
 */
int check_meta_char(char c)
{
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
