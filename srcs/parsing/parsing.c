/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:02:54 by yilin             #+#    #+#             */
/*   Updated: 2025/01/06 17:47:31 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// REDIR
//  * - Check if the first token is PIPE -> ERROR
//  * - Loop List, Check if token valid (INFILE, OUTFILE, HEREDOC, APPEND)
//  * Ensures that a valid filename follows each redirection operator
//  * Handles an error if the redirection is improperly formed (e.g.,
//  * 		missing filename after redirection operator or misplaced PIPE).
int	prs_handle_redir(t_token *token)
{
	if (token && token->type == PIPE)
		return (FAILURE);
	while (token)
	{
		if (token->type == INFILE || token->type == OUTFILE
			|| token->type == HEREDOC || token->type == APPEND)
		{
			if (token->next == NULL || token->next->type != STR)
				return (FAILURE);
			token->next->type = FILENAME;
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
	while (token != NULL)
	{
		if (token->type == STR)
		{
			token->type = COMMAND;
			while (token != NULL && token->type != PIPE)
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

/** GENERATE RANDOM FILENAME
 * 1. Function takes a string pointer
 * 2. Generates a filename in format "/tmp/hd_xxxxxxxx"
 * where x are random lowercase letters a-z
 *
 * @note
 * - Uses Linear Congruential Generator formula:
 *   rand = rand * 1103515245 + 12345
 * 	(Linear congruential generator)
 * - Starting seed is the input string pointer value cast to unsigned long
 * *Security note: Not cryptographically secure due to predictable randomness
 */
static char	*generate_random_filename(char *str)
{
	char			*new;
	int				i;
	unsigned long	rand;

	if (!str)
		return (NULL);
	new = malloc(sizeof(char) * 17);
	if (!new)
		return (NULL);
	i = 0;
	while (i < 8)
	{
		new[i] = "/tmp/hd_"[i];
		i++;
	}
	rand = (unsigned long)str;
	i = 8;
	while (i < 16)
	{
		rand = rand * 1103515245 + 12345;
		new[i] = 'a' + (rand % 26);
		i++;
	}
	new[16] = '\0';
	return (new);
}

// HANDLE HEREDOC
//  * @param char *filename; Variable to store the name of the generated file
//  * @param int fd; File descriptor for the temporary file
//  * @param int end; Flag to indicate an error (if set to 1)
//  * (1) Initialize the error flag to 0 (no error)
//  * (2) Loop token list -> check if it's HEREDOC
//  * -1  Generate a unique filename based on the heredoc content
//  * -2  open() //6: 4+2(Owner can read + write); 4: Group can read
//  * -3  Initialize the heredoc file with input content (if error -> end = 1)
//  * -4  close(fd); + free() (Free the original value of next token and replace
//  * 		it with the generated filename)
//  * -5  Mark this token as NON_TERM_HEREDOC to indicate it has been processed
//  * (3) Return end, which is 0 if successful or 1 if an error occurred
int	prs_handle_heredoc(t_token *token)
{
	char	*filename;
	int		fd;
	int		end;

	end = 0;
	while (token != NULL && end == 0)
	{
		if (token->type == HEREDOC)
		{
			filename = generate_random_filename(token->next->value);
				// generate random -> generate random file
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (!fd)
				return (FAILURE);
			if (prs_init_heredoc(fd, token->next->value) != 0)
				end = 1;
			close(fd);
			free(token->next->value);
			token->next->value = filename;
			token->type = NON_HEREDOC;
		}
		token = token->next;
	}
	return (end);
}

/** INIT HEREDOC
 *
 * (1) Set up a signal handler for SIGINT,
	which will execute `sig_heredoc` when Ctrl+C is pressed.
 * (2) Start infinite loop
 * -1 Display a "heredoc>" prompt and wait for the user to enter a line of text.
 * -2 If the user enters Ctrl+D (end-of-file), `line` will be NULL.
	-> print error -> exit the loop
 *
	-3 Check if (input matches the end-of-file marker) OR (signal to end heredoc has been triggered)
	-> free -> exit
 *
	-4 Write the line to the file descriptor `fd` (where heredoc output is being saved)
 * -5 Add a newline after each line in the output file
 * -6 Free line buffer for the next iteration
 * (3) Check if the `end_heredoc` flag in `g_signals` is set to 1,
 * 		Indicate heredoc process was interrupted (e.g., by a signal like Ctrl+C)
 *
	-> Reset the `end_heredoc` flag to 0 so it’s ready for future heredoc operations.
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
		if (ft_strcmp(line, eof_delimiter) == 0 || g_signal.end_heredoc == 1)
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
		g_signal.end_heredoc = 0;
		return (FAILURE);
	}
	return (SUCCESS);
}

/** REMOVE NODE NULL
 * (1) Initialize 'token' to point to the head of the list
 * (2) Loop & handle NULL values at the beginning of the list: Loop and delete

	* (3) Loop & handle NULL not at the beginning of the list: Loop and delete; if not NULL
	-> continue loop
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
	while (token != NULL && token->next != NULL)
	{
		if (token->next->value == NULL)
		{
			current = token->next;
			token->next = token->next->next;
			free(current);
		}
		else
			token = token->next;
	}
	return (SUCCESS);
}

/** PARSING */
int	parsing(t_token **token)
{
	int	return_code;

	return_code = 0;
	if (prs_check_quotes_valid(*token) != 0)
		return_code = FAILURE;
	else if (prs_handle_quotes_n_expand_env(*token) != 0)
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
