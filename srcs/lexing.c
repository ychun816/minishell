/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:58:20 by yilin             #+#    #+#             */
/*   Updated: 2024/11/06 17:02:45 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** lex_tokenize_each_wd
 * - Extract and labels individual tokens
 * - Identifies a single token based on its type and length
 * 
 * Scope: lex_tokenize_each_wd is limited to handling one word or symbol at a time.
 * Analogy: As someone reading one word at a time from a sentence without worrying about the full sentence
 * @return single token
 * 
 */
t_token	*lex_tokenize_each_wd(char *str, t_shell *content)
{
	t_token	*new_token;
	t_token_type	token_type;
	int	token_len;	
	
	token_type = get_token_type(str);
	token_len = ft_1token_len(str, token_type);
	new_token = create_token(str, token_len, token_type, content); // Create a new token with the identified properties
	return (new_token);
}

/** LEXING
 * - Processing the Entire Line
 * - Manages the entire line, using `lex_tokenize_each_wd` repeatedly to form a structured list of all tokens
 * - It reads through the entire line, calling lex_tokenize each time it encounters a meaningful word or symbol.
 * - Frees any tokens created so far and returns NULL to indicate an error.
 * 
 * @note 
 * lex_tokenize_each_wd(&(input_line[i]), content);
 * &(input_line[i]) instead of input_line: to iterate and process each character one by one
 * 
 * @return  a linked list of tokens, 
 * representing the entire line as a sequence of categorized words or symbols.
 * 
 */
t_token	*lexing(t_shell *content, char *input_line)
{
	t_token	*token;
	t_token	*current; //to store each token at current position
	int	i;
	
	i = 0;
	token = NULL;

	while (input_line[i])
	{
		// If the character is not a space or tab, process it as a token
		if (input_line[i] == ' ' && input_line[i] == '\t')
			i++;
		else
		{
			current = lex_tokenize_each_wd(&(input_line[i]), content); //&(input_line[i]) ?? 
			if (!current) // If tokenize failed, free previously created tokens and exit
			{
				token_free(token);
				return (NULL);
			}
			token_add_back(&token, current); //&token: pointer to t_token
			i += ft_strlen(current->value);
		}
	}
	return (token);
}


/*test

void	print_token(t_token *token)
{
		while (token)
		{
			printf("TOKEN: %s  |  TOKEN TYPE: %i\n", token->value, token->type);
			token = token->next;
		}
}

int test_lexing(void)
{
	char	*input_line = "echo Hello ; ls | grep test";
	t_shell	*content = NULL;
	t_token	*tokens;
	
	tokens = lexing(content, input_line);
	if (!tokens)
	{
		printf("Error: Tokenization Fail\n");
		return (FAILURE);
	}
	print_token(tokens);
	token_free(tokens);
	return (SUCCESS);
}
*/