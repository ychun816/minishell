#include "minishell.h"

/**
 * Purpose: Creates a default environment variable node based on a predefined constant DEF_ENV.
 * Process:
 * - Calls env_get_id() and env_get_value() to extract the ID and value of the default environment variable.
 * - Calls env_create() to allocate memory for a new t_env structure, initializing it with the extracted ID, value, and a duplicate of the raw string.
 * - Returns the newly created environment variable structure.
 */
void set_default_env(void)
{
	char *env_id;
	char *env_value;
	t_shell *content; // Pointer to hold the new environment variable structure

	env_id = get_env_id(DEFAULT_ENV);
	env_value = get_env_value(DEFAULT_ENV);
	content = create_env(env_id, env_value, content);
	return (content);
}

//USER_ZDOTDIR=/home/yilin
char *get_env_id(char *env_line) //If raw points to the beginning of the string (index 0)
{
	char *equal;

	equal = ft_strchr(env_line, '=');
	if (!env_line)
		return (NULL);
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
	
	equal = ft_strchr(env_line, '=');
	if (!env_line)
		return (NULL);
	if (!equal)
		return (NULL);
	else if (equal - env_line == 0)
		return (NULL);
	return (ft_strdup(env_line, (equal - env_line + 1)));
}

/**
 * Purpose: Allocates memory for a new environment variable node and initializes its fields.
 * Process:
 * - Allocates memory for a t_env structure.
 * - If memory allocation fails, it frees any allocated memory for id and value and returns NULL.
 * - Initializes the new structure’s id, value, and raw fields with the provided arguments, and sets the next pointer to NULL.
 * - Returns a pointer to the newly created environment variable structure.
 */

t_shell *create_env(char *env_id, char *env_value, char *env_line)
{
	t_shell *new;

	new = malloc(sizeof(t_shell));//Malloc memory for new_env
	if (!new)//Check if memory fail -> if fail -> free, return NULL
	{
		free(env_id);
		free(env_value);
		return (NULL);
	}
	//Set the ID, value, raw string, and next pointer for the new environment variable structure
	new->env_id = env_id; // Set the variable ID (e.g., "PATH")
	new->env_value = env_value;  // Set the variable value (e.g., "/usr/bin")
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

int env_add_back(t_shell *head, t_shell *new)
{
	t_shell *current;

	if (*head == NULL)
		*head = new;
	else
	{
		current = *head;
		while (current->next != NULL) //if not the last node
			current = current->next; //keep looping
		current->next = new; //add to the last
	}
	return (0);
}
