/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:08:18 by yilin             #+#    #+#             */
/*   Updated: 2024/11/05 18:51:06 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		while (!current->next)
			current = current->next; //loop til end
		current->next = new_token; // Add the new token at the end
	}
	return (SUCCESS);
}