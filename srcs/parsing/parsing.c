/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:02:54 by yilin             #+#    #+#             */
/*   Updated: 2025/01/07 18:30:18 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** CHECK ALL NODES NULL
 * Checks if all nodes in the linked list have NULL values,
 * @return 1 if found NULL value
 * @return 0 if found non-NULL value.
 */
int	prs_check_allnodes_null(t_token *token)
{
	while (token)
	{
		if (token->value)
			return (SUCCESS);
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

/** PRS_REMOVE_NODE_NULL
 * (1) Initialize 'token' to point to the head of the list
 * (2) Loop & handle NULL values at the beginning of the list -> Loop + delete
 * (3) Loop & handle NULL not at the beginning of the list -> Loop + delete;
 *     If not NULL-> continue loop
 * @note *head = token;
 * Update head pointer to new start of linked list after remove initial NULL node
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

/** PRS_HANDLE_CMD
 * - (1) Loop to check if it's string -> Mark as command
 * - (2) Process any tokens that follow until a PIPE or the end of the list
 * - (3) If it's STR (an argument) -> mark as "ARG" type
 * @note 
 *  if (token != NULL) : extra protection conditon at end of while loop
 * -> prevent looping til one after NULL
 * EX:
 * echo "Hello, world" > output.txt | cat -n output.txt
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

/** PRS_UNLINK ERROR
 * - Iterates through a linked list of t_token nodes,
 * - When encounters a node with a specific type (NON_HEREDOC),
 * -> Unlinks (deletes) a file whose path is stored
 *    in the value field of the next node
 * @note
 * unlink():
 * - Delete temporary files associated with here-documents
 * - Ensures that these temporary files are properly removed
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
