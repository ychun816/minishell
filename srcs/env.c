/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:08 by yilin             #+#    #+#             */
/*   Updated: 2024/11/15 16:26:23 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Purpose: Creates a default environment variable node based on a predefined constant DEF_ENV.
 * Process:
 * - Calls env_get_id() and env_get_value() to extract the ID and value of the default environment variable.
 * - Calls env_create() to allocate memory for a new t_env structure, initializing it with the extracted ID, value, and a duplicate of the raw string.
 * - Returns the newly created environment variable structure.
 */
t_env	*set_default_env(void)
{
	char *env_id;
	char *env_value;
	t_env *def_content; // Pointer to hold the new environment variable structure

	env_id = get_env_id(DEFAULT_ENV);
	env_value = get_env_value(DEFAULT_ENV);
	def_content = create_env(env_id, env_value, ft_strdup(DEFAULT_ENV));
	return (def_content);
}

//USER_ZDOTDIR=/home/yilin
char *get_env_id(char *env_line) //If raw points to the beginning of the string (index 0)
{
	char *equal;

	if (!env_line)
		return (NULL);
	equal = ft_strchr(env_line, '=');
	if (!equal)
		return (ft_strdup(env_line));
	else if (equal - env_line == 0) //invalid id(= at first place)
		return (NULL);
	return (ft_strndup(env_line, equal - env_line)); //return substring uptil '='
}

/**
env_line:    V  A  R  =  v  a  l  u  e
       		 0  1  2  3  4  5  6  7  8
        	 ^
       	 env_line
        	 ^
       		equal 0
*/
char *get_env_value(char *env_line)
{
	char *equal;
	
	if (!env_line)
		return (NULL);
	equal = ft_strchr(env_line, '=');
	if (!equal)
		return (NULL);
	else if (equal - env_line == 0)
		return (NULL);
	return (ft_strdup(env_line + (equal - env_line + 1)));
}

/**
 * Purpose: Allocates memory for a new environment variable node and initializes its fields.
 * Process:
 * - Allocates memory for a t_env structure.
 * - If memory allocation fails, it frees any allocated memory for id and value and returns NULL.
 * - Initializes the new structure’s id, value, and raw fields with the provided arguments, and sets the next pointer to NULL.
 * - Returns a pointer to the newly created environment variable structure.
 */

t_env *create_env(char *env_id, char *env_value, char *env_line)
{
	t_env *new;

	new = malloc(sizeof(t_env));//Malloc memory for new_env
	if (!new)//Check if memory fail -> if fail -> free, return NULL
	{
		free(env_id);
		free(env_value);
		return (NULL);
	}
	//Set the ID, value, raw string, and next pointer for the new environment variable structure
	new->id = env_id; // Set the variable ID (e.g., "PATH")
	new->value = env_value;  // Set the variable value (e.g., "/usr/bin")
	new->env_line = env_line; // Set the raw input string (e.g., "PATH=/usr/bin")
	new->next = NULL; // Initialize the 'next' pointer to NULL, as it's a new node
	return (new);
}

/**
 * Purpose: Adds a new environment variable node to the end of a linked list of environment variables.
 * Process:
 * - If the list is empty (i.e., *head is NULL), it sets *head to point to the new node.
 * - If the list is not empty, it traverses to the last node of the list.
 * - Links the new node to the end of the list by setting the next pointer of the last node to point to the new node.
 * - Returns 0 to indicate success.
 */

int env_add_back(t_env **head, t_env *new)
{
	t_env *current;

	if (*head == NULL)
		*head = new;
	else
	{
		current = *head;
		while (current->next != NULL) //if not the last node
			current = current->next; //keep looping
		current->next = new; //add to the last
	}
	return (SUCCESS);
}

/** ENV FREE
 * 
 */
void	env_free(t_env *env)
{
	t_env	*tmp_next;

	while(env)
	{
		tmp_next = env->next;
		if (env->id)
			free(env->id);
		if (env->value)
			free(env->value);
		if (env->env_line)
			free(env->env_line);
		free(env);
		env = tmp_next; //Move to the next node
	}
}

/**FOR PASING- GET ENV
 * Search for an environment variable by its name (path) in a linked list of environment variables (envp).
 * 
*/
t_env	*get_env(char *pathname, t_env *env)
{
	//chek if either is NULL
	if (pathname == NULL || env == NULL)
		return (NULL);
	while (env != NULL)
	{
		if (ft_strcmp(pathname, env->id) == 0)
			return (env);		
		env = env->next;
	}
	return (NULL);
}

/*
The line `env = next;` (or equivalently `env = env->next;`) is crucial,
because it **advances the pointer to the next node** in a linked list, 
allowing the loop to continue processing each node one by one. 
###
In a linked list, each node has a pointer to the next node (`next`), forming a chain. 
By setting `env = next;`, you are effectively **moving down the chain** of nodes, 
which is essential in traversal or cleanup operations.

##
Without `env = next;`, the loop would repeatedly process the same node (the one pointed to by `env`) without moving forward. 
This would cause:
1. An **infinite loop** if the condition to stop relies on reaching the end of the list (`env == NULL`).
2. Incomplete processing or cleanup, as the loop would not reach subsequent nodes in the list.

### 
How `env = next;` Works in the Context of Freeing a Linked List

When you free each node in a linked list, you:
1. **Access the Next Node**: Store `env->next` in a temporary variable (like `next` or `tmp`) before freeing `env`, so you know where the next node is.
2. **Free the Current Node**: Deallocate the memory associated with `env`.
3. **Move to the Next Node**: `env = next;` updates `env` to point to the next node, allowing the loop to continue and repeat the process on each subsequent node.

### 
Example:

```c
while (env)
{
    t_env *next = env->next; // Step 1: Store the next node
    free(env);               // Step 2: Free the current node
    env = next;              // Step 3: Move to the next node
}
```

Each iteration:
- **Step 1** temporarily saves the address of the next node.
- **Step 2** frees the current node.
- **Step 3** moves to the saved next node, ensuring the loop advances through the list.

Without `env = next;`, the pointer `env` would never progress beyond the first node, 
and only the first node would be freed repeatedly, 
resulting in an infinite loop and memory leaks for the remaining nodes in the list.

*/