/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:54:39 by yilin             #+#    #+#             */
/*   Updated: 2024/12/23 15:53:03 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/** UNSET (with no options)
 * unset VAR1 VAR2 VAR3
 * 
*/
int	ft_unset(t_shell *content, t_arg *args)
{
	if (!content->env || ft_env_lstsize(content->env) == 0)
		return (SUCCESS);
	if (!args)
		return (SUCCESS);
	else
	{
		while (args)
		{
			if (delete_env_var(args->value, &(content->env)) != 0)
				return (FAILURE);
			args = args->next;
		}
	}
	return (SUCCESS);
}

/** delete env var 
 * BEFORE:
 * HEAD -> [USER=lin] -> [PATH=/bin] -> [SHELL=/bin/bash] -> NULL
 * AFTER:
 * HEAD -> [USER=lin] -> [SHELL=/bin/bash] -> NULL
 * 
 * (1) Find the node matching 'value' in the environment list
 * (2) If the node does not exist, return 0 (nothing to delete)
 * (3) If the node to delete == the first node in list,
 * => Update the head of the list to skip the node to be deleted
 * (4) If the node to delete != the first node in list,
 * => Skip to unlink the node
 * 
*/
int	delete_env_var(char *env_value, t_env **env)
{
	t_env	*current;
	t_env	*to_delete;

	to_delete = get_env(env_value, *env);
	current = *env;
	if (!(*env))
		return (SUCCESS);
	if (!current)
		return (SUCCESS);
	if (to_delete == *env)
	{
		if (current->next)
		{
			current = current->next;
			*env = current;
		}
	}
	else
	{
		current = *env;
		while (current->next != to_delete)
			current = current->next;
		if (current->next->next)
			current->next = current->next->next;
		else
			current->next = NULL;
	}
	env_delete_1node(to_delete);
	return (SUCCESS);
}

/** env_delete_1node
 * 
 * @note
 * - Safe to set if() conditions -> Avoid freeing NULL nodes
 * - free(env) at last 
 * -> Remove the structure itself, de-allocate the pointer -> prevent memory leak
 */
void	env_delete_1node(t_env *env)
{
	if (env->id)
		free(env->id);
	if (env->value)
		free(env->value);
	if (env->env_line)
		free(env->env_line);
	free(env);
}
