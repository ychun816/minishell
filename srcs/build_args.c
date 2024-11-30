/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 18:31:05 by yilin             #+#    #+#             */
/*   Updated: 2024/11/27 16:08:28 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** bd_handle_args();
 * Processes tokens representing arguments to a command (e.g., filenames, options).
 * Creates and appends each argument to the exec->args list.
 * 
 */
int	bd_handle_args(t_exec *exec, t_token *token) //static int
{
	t_arg	*new;
	
	new = NULL;
	if (token->type == ARG) //handle current token, no need to check the next token
	{
		new = arg_create(token->value);
		if (!new)
			return (-1);
		arg_add_back(&(exec->args), new);
	}
	return(0);
}

/** BUILD - ARG CREATE */
t_arg	*arg_create(char *arg_value)
{
	t_arg	*args;
	char	*dup_arg;

	args = malloc(sizeof(t_arg));
	if (!args)
		return (NULL);
	dup_arg = ft_strdup(arg_value);
	if (!dup_arg)
	{
		free(args);
		return (NULL);
	}
	args->value = dup_arg;
	args->next = NULL;
	return (args);
}

/** BUILD - ARG ADD BACK */
int	arg_add_back(t_arg **head, t_arg *new)
{
	t_arg	*current;
	
	if (new == NULL)
		return (FAILURE); //1
	if (*head == NULL)
		*head = new;
	else
	{
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
	return (SUCCESS);//0
}

/** BUILD - ARG FREE */
void	arg_free(t_arg *args)
{
	t_arg	*tmp;
	
	while (args != NULL)
	{
		tmp = args;
		if (args->value)
			free(args->value);
		args = args->next;
		free(tmp);
	}
}

/** BUILD - ARG LISTSIZE */

