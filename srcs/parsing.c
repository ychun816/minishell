/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:02:54 by yilin             #+#    #+#             */
/*   Updated: 2024/11/22 18:05:35 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** QUOTES
 * validation step -> prevent errors caused by unclosed or invalid quotes before running the more complex functions.
 * 
 * @note *EXTRA CONDITION after ft_quotes_len(), 
 * 			if (str[i] == '\0')
 * 				return (FAILURE);
 *  -> to detect unmatched or missing closing quotes.
 *  
 * If the function encounters the end of the string ('\0') after processing a quote, but without finding a matching closing quote, it means that the quotes are improperly terminated. 
 * This can happen if:

    An opening quote exists (' or "), but there is no corresponding closing quote at the end of the string.
	The string is incomplete or malformed.
	
 */
int	prs_check_quotes_valid(t_token *token)
{
	int i;
	char *str;

	while (token != NULL)
	{
		if (token->type == STR)
		{
			i = 0;
			str = token->value;
			while (str[i])
			{
				if (str[i] == '\'' || str[i] == '\"') // If a quote is found, process it
					i += ft_quotes_len(&(str[i]), str[i]);//incremet i to the end of unquote
				if (str[i] == '\0')
					return (FAILURE);
			}
			i++;
		}
		token = token->next;
	}
	return (SUCCESS);
}
/** prs_get_quoted_str
 * (1) Calculate the length of the quoted string (excluding the closing quote).
 * (2)  Default to DOUBLEQUOTE type
 * (3) Check if inside the quote has context
 *    [IF] has context  -> Create a token starting after the opening quote (`str + 1`) of the given length
 * 	  [ELSE] NO context -> Inside the quotes, create an empty string token.
 * 
 * @note
 * len = ft_quotes_len(input_str, c) - 1; => skip closing quote (ft_quotes_len)
 * new_token = creat_token(input_str + 1, len, type, content); => skip opening quote (input_str + 1)
 * new_token = creat_token("\0", 1, STR, content):
 * "\0": represent an empty string (null-terminated), a string with no content.
 * STR: This type indicates that it is just a regular string (not quoted or other types).
 * 
 */
t_token	*prs_get_quoted_str(char *input_str, char c, t_shell *content)
{
	int	len;
	t_token	*new_token;
	t_token_type	type;
	
	len = ft_quotes_len(input_str, c) - 1; 
	new_token = NULL;
	type = DBL_QUOTE;
	if (c == '\'')
		type = SGL_QUOTE;	
	if (len > 0)
		new_token = create_token(input_str + 1, len, type, content);
	else
		new_token = create_token("\0", 1, STR,  content);
	return (new_token);	
}

/** ft_rogue_len
 * 
 */
int	ft_rogue_len(char	*str)
{
	int	len;
	
	len = 0;
	while (str[len])
	{
		if (str[len] == '\'' || str[len] == '\"')
			break ;
		len++;
	}
	return (len);
}


/** prs_quotes_to_tokens
 * Parses a string into tokens by:
 * - Identifying quoted and unquoted parts of the input string.
 * - Creating tokens for each segment (quoted or unquoted).
 * - Returning a linked list of tokens representing the parsed segments.
 * 
 * (1) Loop input_str
 * 		[IF] the current character is a single or double quote
 * 		-> Extracts the content inside the quotes + creates a token for it
 * 		-> Skip the closing quote by moving the index `i` forward by the length of the processed string + 1
 * 		[ELSE] not quotes
 * 		-> Create a raw token for the unquoted part
 * 		-> Move `i` forward by the length of the raw segment minus 1
 *   	 (since the `i++` at the end of the loop increments it further)
 * (2) If token creation failed -> return NULL
 * (3) Add the newly created token to the list of tokens
 * @return result (newly created token)
 */
t_token	*prs_quotes_to_tokens(char *input_str, t_shell *content)
{
	int	i;
	t_token	*token;
	t_token	*new_token;

	i = 0;
	token = NULL;
	new_token = NULL;

	while (input_str[i])
	{
		if (input_str[i] == '\'' || input_str[i] == '\"')
		{
			new_token =  prs_get_quoted_str(&(input_str[i]), input_str[i], content);
			i += ft_strlen(new_token->value) + 1;
		}
		else
		{
			new_token = create_token(&(input_str[i]), ft_rogue_len(&(input_str[i])), STR, content);
			i += ft_strlen(new_token->value) - 1;
		}
		if (!new_token)
			return (NULL);
		token_add_back(&token, new_token);
		i++;
	}
	return (new_token);
}


/**handle quotes n expand env
 * processes tokens in a linked list of tokens (t_token) by handling:
 * 
 * (1) Iterate through all tokens in the linked list (= input string)
 * (2) Process tokens of type STRING:
 * 	   -1 Parse quotes within the token's value and return a list of tokens
 * 	   -2 Expand environment variables within the parsed tokens 
 * 
 */
int	prs_handle_quotes_n_expand_env(t_token *token)
{
	t_token	*input_str;

	while (!token)
	{
		prs_expand_env(input_str);
		if (token->type == STR)
		{
			input_str = prs_quotes_to_tokens(token->value, token->content);
			prs_expand_env(input_str);
			if (token->value)
				free(token->value);
			token->value = prs_tokens_combine(input_str);
			token_free(input_str);	
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
	if (token && token->type == PIPE)
		return (FAILURE);
	while (token)
	{
		if (token->type == INFILE || token->type == OUTFILE || token->type == HEREDOC || token->type == APPEND)
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
			token->type = COMMAND;
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
	while (token != NULL && end == 0)
	{
		if (token->type == HEREDOC)
		{
			filename = NULL;
			//filename = ms_generate_random(token->next->value); //generate random -> generate random file
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (!fd)
				return (FAILURE);
			if (prs_init_heredoc(fd, token->next->value) != 0)
				end = 1;
			close (fd);
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
	
	signal(SIGINT, sig_heredoc);/// RECHECK
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
		g_signal.end_heredoc = 0;
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
			prs_expand_envvar_to_token(token);
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