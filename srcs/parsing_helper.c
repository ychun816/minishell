/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:48:18 by yilin             #+#    #+#             */
/*   Updated: 2024/12/03 19:01:18 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**FOR PASING- GET ENV
 * Search for an environment variable by its name (path) in a linked list of environment variables (envp).
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
			prs_handle_envvar_expansion(token);
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

/* TESTER HELPER */

/**  TEST TOKEN APPEND
 * Append a token to the list 
*/
void test_token_append(t_token **head, char *value, int n, t_token_type type, t_shell *content) 
{
    // Create a new token using token_create
    t_token *new_token = token_create(value, n, type, content);
    if (!new_token) 
	{
        fprintf(stderr, "Failed to create token for value: %s\n", value);
        return;
    }
    // If the list is empty, set the new token as the head
    if (!*head) 
	{
        *head = new_token;
        return;
    }
    // Traverse to the end of the list and append the new token
    t_token *current = *head;
    while (current->next)
        current = current->next;
    current->next = new_token;
}

/** TEST PRINT TOKEN */
void	test_print_tokens(t_token *head) 
{
    while (head) 
	{
        printf("Value: %s, Type: %s\n", head->value ? head->value : "(null)", test_tokentype_to_str(head->type));
        head = head->next;
    }
}

/* tokenize_input */
t_token *test_tokenize_input(char *input, t_shell *shell)
{
    t_token *head = NULL;
	t_token *current = NULL;
    char *token_value;
    size_t i = 0;
	size_t start;

    while (input[i])
    {
        // Skip whitespace
        while (input[i] == ' ' || input[i] == '\t')
            i++;
        // If we encounter a token
        start = i;
        if (input[i] == '>' || input[i] == '<' || input[i] == '|')
        {
            // Handle redirection and pipe
            token_value = strndup(&input[i], 1);
            t_token_type type = (input[i] == '>') ? OUTFILE : (input[i] == '<') ? INFILE : PIPE;
            i++;

            t_token *new_token = token_create(token_value, 1, type, shell);
            free(token_value);
            if (current)
                current->next = new_token;
            else
                head = new_token;
            current = new_token;
        }
        else if (input[i] != '\0')
        {
            // Handle normal strings (e.g., command or argument)
            while (input[i] && input[i] != ' ' && input[i] != '>' && input[i] != '<' && input[i] != '|')
                i++;
            token_value = strndup(&input[start], i - start);
            t_token *new_token = token_create(token_value, i - start, STR, shell);
            free(token_value);

            if (current)
                current->next = new_token;
            else
                head = new_token;
            current = new_token;
        }
    }
    return head;
}

/* token type to str*/
const char *test_tokentype_to_str(t_token_type type)
{
    switch (type)
    {
        case INFILE:    return "INFILE";
        case OUTFILE:   return "OUTFILE";
        case HEREDOC:   return "HEREDOC";
        case NON_HEREDOC:return "NON_HEREDOC";
        case APPEND:    return "APPEND";
        case PIPE:      return "PIPE";
        case STR:       return "STR";
        case SGL_QUOTE: return "SINGLE QUOTE";
        case DBL_QUOTE: return "DOUBLE QUOTE";
        case COMMAND:   return "COMMAND";
        case FILENAME:  return "FILENAME";
        case ARG:       return "ARG";
        default:        return "UNKNOWN";  // In case of invalid type
    }
}