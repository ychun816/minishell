/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:02:54 by yilin             #+#    #+#             */
/*   Updated: 2024/11/15 16:40:11 by yilin            ###   ########.fr       */
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

/** prs_expand_n_quotes
 * 
 */
int	prs_expand_and_quotes(t_token *token)
{
	
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

/** HANDLE HEREDOC 
 * @param char *filename; Variable to store the name of the generated file
 * @param int fd; File descriptor for the temporary file
 * @param int end; Flag to indicate an error (if set to 1)
 * (1) Initialize the error flag to 0 (no error)
 * (2) Loop token list -> check if it's HEREDOC
 * -1  Generate a unique filename based on the heredoc content
 * -2  open() //6: 4+2 (Owner can read and write) ; 4: Group can read ; 4: Group can read
 * -3  Initialize the heredoc file with the input content (if error -> end = 1)
 * -4  close(fd); + free() (Free the original value of the next token and replace it with the generated filename)
 * -5  Mark this token as NON_TERM_HEREDOC to indicate it has been processed
 * (3) Return end, which is 0 if successful or 1 if an error occurred
 * 
*/
int	prs_handle_heredoc(t_token *token)
{
	char	*filename;
	int	fd;
	int	end;

	end = 0;
	while (token && end == 0)
	{
		if (token->type == HEREDOC)
		{
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (!fd)
				return (FAILURE);
			if (prs_init_heredoc(fd, token->next->value) != 0)
				end = 1;
			close (fd);
			free(token->next->value);
			token->next->value =filename;
			token->type = NON_HEREDOC;
		}
		token = token->next;
	}
	return (end);
}

/** INIT HEREDOC
 * 
 * (1) Set up a signal handler for SIGINT, which will execute `sig_heredoc` when Ctrl+C is pressed.
 * (2) Start infinite loop
 * -1 Display a "heredoc>" prompt and wait for the user to enter a line of text.
 * -2 If the user enters Ctrl+D (end-of-file), `line` will be NULL. -> print error -> exit the loop
 * -3 Check if (input matches the end-of-file marker) OR (signal to end heredoc has been triggered) -> free -> exit
 * -4 Write the line to the file descriptor `fd` (where heredoc output is being saved)
 * -5 Add a newline after each line in the output file
 * -6 Free line buffer for the next iteration
 * (3) Check if the `end_heredoc` flag in `g_signals` is set to 1, 
 * 	   Indicate heredoc process was interrupted (e.g., by a signal like Ctrl+C)
 * -> Reset the `end_heredoc` flag to 0 so it’s ready for future heredoc operations.
 * 
 */
int	prs_init_heredoc(int fd, char *eof_delimiter)
{
	char	*line; 
	
	signal(SIGINT, sig_heredoc);
	while (1) 
	{
		line = readline("heredoc>");
		if (!line)
		{
			ft_putstr_fd("here_doc: called end-of-line (ctrl-d)\n", 2);
			break ;
		}
		if (ft_strcmp(line, eof_delimiter) != 0 || g_signal.end_heredoc == 1)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	if (g_signal.end_heredoc == 1)
	{
		g_signal.end_heredoc == 0;
		return (FAILURE); //1
	}
	return (SUCCESS); //0
}


/** REMOVE NODE NULL
 * (1) Initialize 'token' to point to the head of the list
 * (2) Loop & handle NULL values at the beginning of the list: Loop and delete
 * (3) Loop & handle NULL not at the beginning of the list: Loop and delete; if not NULL -> continue loop
 * 
 * @note *head = token;
 * it updates the head pointer to the new start of the linked list after removing any initial nodes with NULL values.
 * 
 */
int	prs_remove_node_null(t_token **head)
{
	t_token	*current;
	t_token	*token;
	
	token = (*head);

	while (token != NULL && token->value == NULL)
	{
		current = token;
		token = token->next;
		free(current);
	}
	*head = token;
	while (token != NULL && token->value != NULL)
	{
		if (token->next->value == NULL)
		{
			current = token->next;
			token->next = token->next->next; // Bypass the node with NULL value, linking to the following node.
			free(current);
		}
		else
			token = token->next; // Move to the next node only if no deletion occurred.
	}
	return (SUCCESS);
}

/** CHECK ALL NODES NULL
 * Checks if all nodes in the linked list have NULL values, 
 * 
 * @return 1 if found NULL value
 * @return 0 if found non-NULL value.
 * 
 */
int	prs_check_allnodes_null(t_token *token)
{
	while (token)
	{
		if (token->value)
			return (0);
		token = token->next;
	}
	return (1);
}

/** UNLINK ERROR
 * Iterates through a linked list of t_token nodes,
 * When it encounters a node with a specific type (NON_HEREDOC), 
 * It unlinks (deletes) a file whose path is stored in the value field of the next node. 
 * 
 * @note 
 * unlink(): 
 * - Delete temporary files associated with here-documents.
 * - Ensures that these temporary files are properly removed.
 */
void	prs_unlink_error(t_token *token)
{	
	while (token)
	{
		if (token->type == NON_HEREDOC)
			unlink(token->next->value);
		token = token->next;
	}
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
	else if (prs_remove_node_null(token) != 0)
		return_code = FAILURE;
	else if (prs_check_allnodes_null(*token) != 0)
		return_code = FAILURE_VOID;
	else if (prs_handle_redir(*token) != 0)
		return_code = FAILURE;
	else if (prs_handle_cmd(*token) != 0)
		return_code = FAILURE;
	else if (prs_handle_heredoc(*token) != 0)
	{
		prs_unlink_error(*token);
		return_code = FAILURE_VOID;
	}
	return (return_code);
}