/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:08:18 by yilin             #+#    #+#             */
/*   Updated: 2024/11/28 12:19:26 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** CREATE TOKEN
 * - Creates a new token and initializes its properties based on the provided values.
 * - (1) Allocates Memory:  for a new token structure to hold the token’s data
 * - (2) Dup input txt by parts: strndup() -> dup and saves this copy as the token’s "value." 
 * - (3) Label the token: t_token_type
 * - (4) Link context: It attaches any additional context, like where it came from, if needed.
 * 
 * @param token_value:  A pointer to the start of the token’s value in the input string.
 * @param n: The number of characters to copy from `value` to the token’s value.
 * @param type: The type of the token (e.g., STRING, COMMAND)
 * @param content: A content pointer (e.g., to provide additional info about the token’s environment)
 * 
 * For example:
 * - token_value: points to the h in "hello" in "echo hello"
 * - n:  n is 5 (the length of "hello"),
 * 
 */
t_token	*token_create(char *token_value, int n, t_token_type type, t_shell *content)
{
	t_token	*new_token;
	char	*dup_token_value;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	dup_token_value = ft_strndup(token_value, n);
	if (!dup_token_value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->value = dup_token_value;
	new_token->type = type;	// Assign the context `content` to the token’s context field
	new_token->content = content;
	new_token->next = NULL;
	return (new_token);
}

/** TOKEN ADD BACK
 * - Adds a new token to the end of a linked list of tokens.
 * - If the list is empty (head is NULL), sets the new token as the head.
 * 
 * @return 0 if successful / 1 if the new token is NULL.
 * 
 */
int	token_add_back(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return (FAILURE);
	if (!(*head))
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next; //loop til end
		current->next = new_token; // Add the new token at the end
	}
	return (SUCCESS);
}

/**TOKEN FREE 
 * Frees an entire linked list of tokens: 
 * - (1) value inside each token(node)
 * - (2) pointer to linked list
 * 
*/
void	token_free(t_token *token)
{
	t_token	*current;

	current = NULL;
	while (current != NULL)
	{
		current = token; //current to tmperorily save token pointer
		current = token->next;
		free(current->value);	
	}
	free(current);
}