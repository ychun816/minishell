/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:44:33 by yilin             #+#    #+#             */
/*   Updated: 2024/12/06 18:25:24 by yilin            ###   ########.fr       */
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


/* TEST READ N LOOP*/				// ft_putstr_fd("Parsing Error!\n", 2);
int test_read_n_loop(t_shell *content)
{
	char *line;
	t_token	*lexed_tokens;
	t_exec	*result_exec;

	// (void)content;
	line = NULL;
	while (1)
	{
		// sig_init_signals(); // Set up signal handling for each loop iteration
		line = readline(PROMPT);
		if (!line)// EOF or NULL input
			break ;// EOF or NULL input
		if (strncmp(line, "env", 3) == 0) //////////TESTER//////////
			display_env(content->env);
		if (strncmp(line, "exit", 4) == 0) //////////TESTER//////////
			break ;
		else if (check_line_empty(line) == 0)// check if line valid
		{
			add_history(line);//add history //REMEMBER TO CLEAR
			// if (process_pipeline(content, line) != 0)//process pipeline
			// {
			// 	ft_putstr_fd("Parcing Error\n", 2);
			// 	content->exit_code = 2;
			// }

			/// STEP 1: LEXING ///
			lexed_tokens = lexing(content, line);
			if (!lexed_tokens)
			{
				ft_putstr_fd("Lexing Error\n", 2);
				content->exit_code = 1;
				free(line);
				continue ;// Skip parsing and execution for this input
			}
			else
			{
				printf("\n=== 🍿 LEXED TOKENS ===\n");
				test_print_tokens(lexed_tokens);
				// Continue to parsing, no need continue
			}
			
			/// STEP 2: PARSING ///
			if (parsing(&lexed_tokens) != 0) //parsing fail
			{
				// ft_putstr_fd("Parsing Error!\n", 2);
    			if (parsing(&lexed_tokens) == FAILURE)
    			    printf("🢂 👎 Parsing Failed (General Error)!\n");
    			else if (parsing(&lexed_tokens) == FAILURE_VOID)
    			    printf("🢂 🫶 Parsing Failed (Failure void error)!\n");
				content->exit_code = 2;
				token_free(lexed_tokens);
				free(line);
				continue ;
			}
			else
			{
				printf("\n=== 🥐 TEST PARSING ===\n");
				test_print_tokens(lexed_tokens);
				// if (result == 0) //SUCCESS
				printf("🢂 🤙 Parsing Successful!!\n");
			}

			/// STEP 3: BUILD TO EXEC ///
			result_exec = build_for_exec(lexed_tokens);
			if (!result_exec)
			{
				ft_putstr_fd("Execution Preparation Error\n", 2);
				content->exit_code = 3;
			}
			else
			{
				printf("\n=== 🍰 TEST EXEC ===\n");
				test_print_exec(result_exec);
				build_free_all(result_exec);
			}
			token_free(lexed_tokens);
		}
		if (line)
			free(line);
	}
	return (0); //return if loop end ((i.e., on EOF or NULL input))
}