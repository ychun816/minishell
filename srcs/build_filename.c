/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_filename.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 18:28:46 by yilin             #+#    #+#             */
/*   Updated: 2024/11/23 20:38:48 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**	bd_handle_redirs();
 * 
 * Adds these redirections to the redirs list in the t_exec structure.
 * Processes tokens representing redirections (e.g., >, >>, <, <<).
 * Creates and appends a redirection entry (target file or heredoc content) to the exec->redirs list.
 * 
 * (1) Check if:
 * - The token has a valid next token (the filename or target of redirection)
 * - The current token type is a redirection type (APPEND, HEREDOC, INFILE, OUTFILE)
 * (2) Create a new redirection file structure using the next token's value (the next token of the redir sign)
 * (3) Add to redirs list
 * 
 * @note
 * 
 * 
 */
int	bd_handle_redirs(t_exec *exec, t_token *token) //static int
{
	t_filename	*tmp;

	if (token->next && token->next->value != NULL && (token->type == APPEND || token->type == HEREDOC || token->type == INFILE || token->type == OUTFILE))
	{
		tmp = filename_create(token->next->value, token->type);
		if (!tmp)
			return (-1); //FAILURE_VOID (defined as 2) -> Specific failure: memory allocation
		filename_add_back(&(exec->redirs), tmp);
		return (1); //FAILURE -> Indicating success in handling redirection
	}	
	return (0); //SUCCESS -> no action taken
}

/** BUILD- FILENAME CREATE
 * Creates a structured object (t_filenames) that stores the file path and the type of redirection
 * 
 * @param  t_filenames *filename;  // Declare a pointer to a t_filenames structure to hold the redirection information
 * @param  char *new;  // Declare a pointer to store the duplicated string (path)
 * 
 * Example:
 * For < input.txt:
 * Call filename_create("input.txt", INFILE) to create a structure with the file path "input.txt" and type INFILE.
 * 
 * For > output.txt:
 * Call filename_create("output.txt", OUTFILE) to create a structure with the file path "output.txt" and type OUTFILE.
 * 
 */
t_filename *filename_create(char *pathname, t_token_type type)
{
	t_filename *filename;
	char	*dup_path;

	filename = malloc(sizeof(t_filename));
	if (!filename)
		return (NULL);
	dup_path = ft_strdup(pathname);
	filename->path = dup_path;
	filename->type = type;
	filename->next = NULL;
	// Initialize the next pointer to NULL (this will link to the next t_filenames structure in a linked list, if needed)
	return (filename);
}

/** BUILD - FILENAME ADD BACK
 * @note
 * No need to set NULL after new, cuz in file_creat() already attach NULL to new node
*/
int	filename_add_back(t_filename **head, t_filename *new)
{
	t_filename *current;
	
	if (!new)
		return (FAILURE); //1
	if (*head == NULL)
		*head = new;
	else
	{
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new;// Add the new node after the last node
	}	
	return (SUCCESS); //0
}

/** BUULD - FILENAME FREE */
void	filename_free(t_filename *filename)
{
	t_filename	*tmp;
	
	while (filename != NULL)
	{
		tmp = filename;
		if (filename->path)
			free(filename->path);
		filename = filename->next;
		free(tmp);
	}
}

/** BUILD - FILENAME LISTSIZE */