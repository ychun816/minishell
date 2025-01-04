/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:04:26 by yilin             #+#    #+#             */
/*   Updated: 2024/12/16 18:03:11 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** 42 subject for builtins:
 * - pwd with no options
 * - export with no options
 * - unset with no options
 * - env with no options or arguments
 * - exit with no options
 * - echo with option -n
 * - cd with only a relative or absolute path
 */

#include "builtins.h"

/** CHECK IS BUILTIN **/
int	check_is_builtin(char* input_cmd)
{
	if (!input_cmd)
		return (0);
	else if (ft_strcmp(input_cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "exit") == 0)
		return (2);
	else
		return (0);
}

/** EXEC BUILTIN **/
int	exec_builtin(t_shell *content, char* input_cmd, t_arg *args)
{
	if (ft_strcmp(input_cmd, "echo") == 0)
		return (ft_echo(args));
	else if (ft_strcmp(input_cmd, "cd") == 0)
		return (ft_cd(content, args));
	else if (ft_strcmp(input_cmd, "pwd") == 0)
		return (ft_pwd(args));
	else if (ft_strcmp(input_cmd, "export") == 0)
		return (ft_export(content, args));
	else if (ft_strcmp(input_cmd, "unset") == 0)
		return (ft_unset(content, args));
	else if (ft_strcmp(input_cmd, "env") == 0)
		return (ft_env(content, args));
	else if (ft_strcmp(input_cmd, "exit") == 0)
		return (ft_exit(content, args));
	else
		return (0);
}

/** ADD ENVVAR
 * Adds or updates an environment variable
 * Ensures that the variable's name and value are valid
 * Creates a new environment entry or updates an existing one.
 * 
 * @param
 * char	*arg_id;         // The name/ID of the environment variable.
 * char	*arg_value_dup;  // Duplicate of the value string.
 * t_env	*tmp; dd_envvar(char *input_line, t_env **env_head)
{
	char	*env_id;
	char	*env_value;
	char	*dup_inputlinenv not exist => Create new one (initialize + add end-node)
 * -> If env exit => Update the existing variable.
 * 
*/
/* OG
int	add_envvar(char *input_line, t_env **env_head)
{
	char	*env_id;
	char	*env_value;
	char	*dup_inputline;
	t_env	*env;
	
	env_id = get_env_id(input_line);
	if (!env_id || !check_envid_valid(env_id))
		return (free(env_id), error_export(input_line), FAILURE);
	
	env_value = get_env_value(input_line);
	if (!env_value)
		return (free(env_id), SUCCESS);
	printf("ENV_ID: %s\n", env_id);
	dup_inputline = ft_strdup(input_line);
	printf("can i come here");
	printf("DUP_INPUTLINE: %s\n", dup_inputline);
	if (!dup_inputline)
		return (free(env_id), FAILURE);
	env = get_env(dup_inputline, *env_head);//input_line
	if (!env)
	{
		env = env_create(env_id, env_value, dup_inputline);//input_line
		if (!env)
			return (free(dup_inputline), free(env_id), FAILURE);//input_line
		env_add_back(env_head, env);	
	}
	else
	{
		update_envvar(env, dup_inputline);
		free (env_id);
	}
	// free(dup_inputline);//debug
	return (SUCCESS);
}
*/
/* modified add envvar*/
int	add_envvar(char *input_line, t_env **env_head)
{
	char	*env_id;
	char	*env_value;
	char	*dup_inputline;
	t_env	*env;
	
	env_id = get_env_id(input_line);
	env = get_env(env_id, *env_head);//input_line
	if (!env_id || !check_envid_valid(env_id))
		return (free(env_id), error_export(input_line), FAILURE);
	dup_inputline = ft_strdup(input_line);
	if (!dup_inputline)
		return (free(env_id), FAILURE);

	if (!env)
	{
		env_value = get_env_value(input_line);
		env = env_create(env_id, env_value, dup_inputline);//input_line
		if (!env)
		{
			free(input_line);
			if (env_value)
				free(env_value);
			free(env_id);
			env = NULL;
			return (FAILURE);
		}
		env_add_back(env_head, env);
	}
	else
	{
		free (env_id);
		update_envvar(env, dup_inputline);
	}
	return (SUCCESS);
}

/** CHECK ENVID VALID 
 * (1) Check if env_id empty
 * (2) Check if env_id first character alpha or not '_'
 * (3) Then loop to check if env_id rest of character numeric
 * 
 * @note
 * - Valid: "HOME", "USER_NAME", "_PWD".
 * - Invalid: "123ABC", "$PATH", "!VAR" (
 * because the first character is not a letter or underscore).
 * 
*/
int	check_envid_valid(char *env_id)
{
	int	i;

	i = 0;
	if (env_id[i] == '\0')
    	return (0);
	if (ft_isalpha(env_id[i]) == 0  && env_id[i] != '_')
        return (0);
	i++;
	while (env_id[i])
	{
		if (ft_isalnum(env_id[i]) == 0 && env_id[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/** UPDATE ENV VAR
 * Updates an existing environment variable node's value and its full line representation.
 * It frees old memory for the value and line and assigns new ones.
 * 
 * (1) Retrieve the value associated with the raw environment string
 * (2) If no valid value was found (arg_value is NULL), free the raw string -> exit
 * (3) If the 'node (update_env)' has an existing value => free the old value memory.
 * (4) If the 'node  (update_env)' has an existing environment line (raw string) => free old env_line
 * (5) Assign the new value and environment line to the 'node' (update_env) structure.
 * 
*/
void	update_envvar(t_env *update_env, char *env_line)
{
	char	*env_value;

	env_value = get_env_value(env_line);
	
	if (!env_value)
	{
		free(env_value);
		return ;
	}
	if (update_env->value)
		free(update_env->value);
	if (update_env->env_line)
		free(update_env->env_line);
	update_env->value = env_value;
	update_env->env_line = env_line;
}
