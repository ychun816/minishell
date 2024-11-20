/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:02 by yilin             #+#    #+#             */
/*   Updated: 2024/11/20 16:46:35 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** CREATE TOKEN
 * - Creates a new token and initializes its properties based on the provided values.
 * - (1) Allocates Memory:  for a new token structure to hold the token’s data
 * - (2) Dup input txt by parts: strndup() -> dup and saves this copy as the token’s "value." 
 * - (3) Label the token: t_token_type
 * - (4) Link context: It attaches any additional context, like where it came from, if needed.
 * 
 * @param token_value:  A pointer to the start of the token’s value in the input string.
 * @param n: The number of characters to copy from `value` to the token’s value.
 * @param type: The type of the token (e.g., STRING, COMMAND)
 * @param content: A content pointer (e.g., to provide additional info about the token’s environment)
 * 
 * For example:
 * - token_value: points to the h in "hello" in "echo hello"
 * - n:  n is 5 (the length of "hello"),
 * 
 */
t_token	*create_token(char *token_value, int n, t_token_type type, t_shell *content)
{
	t_token	*new_token;
	char	*dup_token_value;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	
	dup_token_value = ft_strndup(token_value, n);
	if (!dup_token_value)// Check if the duplication failed
	{
		free(new_token); //Free the allocated token if strndup fails
		return (NULL);
	}
	new_token->value = dup_token_value;
	new_token->type = type;
	// Assign the context `content` to the token’s context field
	new_token->content = content;
	// Initialize the `next` pointer to NULL (since it’s the end of the list for now)
	new_token->next = NULL;
	return (new_token);
}

/** FT_TOKEN_LEN
 * - *This len helps to identify how many characters to include in a token.
 * - Count len of a token based on its type
 * @param str: A pointer to the start of the token in the input string.
 * @param type: The type of the token (e.g., HEREDOC, APPEND, INFILE, OUTFILE, PIPE, STRING).
 * 
 * - INFILE (>), OUTFILE (<), or PIPE (|) => len = 1
 * - HEREDOC (<<) or APPEND (>>) => len = 2
 * - STR => ft_token_str_len()
 * 
 */
int	ft_1token_len(char *str, t_token_type type)
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
			len += ft_quotes_len(str, '\'');
		//double quote
		else if (str[len] == '\"')
			len += ft_quotes_len(str, '\"');
		//break if meet a meta character
		else if (check_meta_char(str[len]) != 0 || str[len] == '\t' || str[len] == ' ')
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
