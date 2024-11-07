/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:02:54 by yilin             #+#    #+#             */
/*   Updated: 2024/11/07 17:53:02 by yilin            ###   ########.fr       */
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
 * - Check if the first token is PIPE -> ERROR
 * - Loop List, Check if token valid (INFILE, OUTFILE, HEREDOC, APPEND)
 * - Ensure the next token exists and is of type STRING (representing a filename
 * 
 * Ensures that a valid filename follows each redirection operator
 * Handles an error if the redirection is improperly formed (e.g., missing filename after redirection operator or misplaced PIPE).
 */
int	prs_handle_redir(t_token *token)
{
	if (token && token->value == PIPE)
		return (FAILURE);
	while (token)
	{
		if (token->value == INFILE || token->value == OUTFILE || token->value == HEREDOC || token->value == APPEND)
		{
			if (token->next == NULL || token->next->type != STR)
				return (FAILURE);
			token->next->type = FILENAME; // Change the type of the next token to FILENAME (it represents a file name)
		}
		else if (token->type == PIPE && token->next == NULL)
			return (FAILURE);
		token = token->next;
	}
	return (SUCCESS);
}

/** CMD
 * - (1) Loop to check if it's string -> Mark as command
 * - (2) Process any tokens that follow until a PIPE or the end of the list
 * - (3) If it's a STR (an argument), mark it as an ARG type
 * 
 * @note if (token != NULL) : extra protection conditon at the end of while loop
 * -> prevent looping til one after NULL
 * 
 * EX:
 * echo "Hello, world" > output.txt | cat -n output.txt
 * 
 */
int	prs_handle_cmd(t_token *token)
{
	while (token)
	{
		if (token->type == STR)
		{
			token->type == COMMAND;
			while (token && token->type != PIPE)
			{
				if (token->type == STR)
					token->type = ARG;
				token = token->next;
			}
		}
		if (token != NULL)
			token = token->next;
	}
	return (SUCCESS);
}

/** HEREDOC 
 * 
 * 
 *  
*/
int	prs_handle_heredoc(t_token *token)
{
	char	*filename; // Variable to store the name of the generated file
	int	fd; // File descriptor for the temporary file
	int	end; // Flag to indicate an error (if set to 1)

	end = 0;// Initialize the error flag to 0 (no error)
	while (token && end == 0)
	{
		if (token->type == HEREDOC) //current token is HEREDOC
		{
			// Generate a unique filename based on the heredoc content
			filename = shell_generate_random(token->next->value); 
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (!fd)
				return (FAILURE);
			// Initialize the heredoc file with the input content
			if (prs_init_heredoc(fd, token->next->value) != 0)
				end = 1; // Set end to 1 if ps_init_here_doc encounters an error
			close (fd); // Close the file after writing to it
			free(token->next->value); // Free the original value of the next token and replace it with the generated filename
			token->next->value =filename;
			token->type = NON_TERM_HEREDOC;// Mark this token as N_HEREDOC to indicate it has been processed
		}
		token = token->next;
	}
	return (end);// Return end, which is 0 if successful or 1 if an error occurred
}

/**
 * 
 * 
 */
int	prs_remove_null(t_token *token)
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