/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:44:33 by yilin             #+#    #+#             */
/*   Updated: 2024/12/05 19:23:35 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/******************************/
/*********** PARSING **********/
/******************************/

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
        printf("[Value]: %s | [Type]: %s\n", head->value ? head->value : "(null)", test_tokentype_to_str(head->type));
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

/******************************/
/************ EXEC ***********/
/******************************/

/*TEST EXEC*/
void test_print_exec(t_exec *exec) 
{
    if (!exec) 
	{
        printf("[test_print_exec] FAILURE: Executable is NULL.\n");
        return;
    }

    // Print command
    if (exec->cmd) 
        printf("[Command]: %s\n", exec->cmd);
    else
        printf("No command set.\n");

    // Print arguments
    if (exec->args) 
	{
        t_arg *arg = exec->args;
        while (arg) 
		{
            printf("[Argument]: %s\n", arg->value);
            arg = arg->next;
        }
    }
	else
        printf("No arguments set.\n");

    // Print redirections
    if (exec->redirs) 
	{
        t_filename *redir = exec->redirs;
        while (redir) 
		{
            printf("[Redirection]: %s (type %s)\n", redir->path, test_tokentype_to_str(redir->type));
            redir = redir->next;
        }
    } 
	else 
        printf("No redirections set.\n");
}