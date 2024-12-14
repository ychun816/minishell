/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:56:29 by yilin             #+#    #+#             */
/*   Updated: 2024/12/14 18:26:03 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"


/** CD (relative or absolute path)
 * 1. Get the number of arguments passed to 'cd'
 * 2. If more than 1 args -> "error: too many arguments"
 * 3. Get the current working directory and store it in variable, if fail-> perror
 * 4. Get the 'HOME' environment variable
 * 5. 
 * 
 * 
*/
int	ft_cd(t_shell *content, t_arg *args)
{
	int	args_nb;
	char	*cwd;
	t_env	*home;
	
	args_nb = ft_arg_lstsize(args);
	if (args_nb > 1)
		return (ft_putstr_fd("minishell: cd: too many arguments\n",
				STDERR_FILENO), 1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell: cd: error retrieving current directory");
	home = get_env("HOME", content->env);
	//take care of "--"
	if ((!args_nb || !ft_strcmp(args->value, "--")) && home && home->value)
		chdir(home->value);
	else if ((!args_nb || !ft_strcmp(args->value, "--")) && (!home || !home->value))
		return (error_cd(errno, "HOME"), free(cwd), 1);
	else if (chdir(args->value) < 0)
	{
		error_cd(errno, args->value);
		return (free(cwd), 1);
	}
	if (update_pwd(content, cwd))
		return (free(cwd), 1);		
	return (free(cwd), SUCCESS);
}
/** UPDATE PWD 
 * Updates the PWD (current working directory) and OLDPWD (previous working directory) environment variables. * 
 * 
 * @param
 * - t_shell *content: A pointer to the shell's context structure containing the environment.
 * - t_env *old_pwd / pwd: interact wiht the environment
 * - char *current_cwd: Directly holds the raw directory path returned by getcwd
 * - char *pwd_path
 * - char *oldpwd_path
 * 
 * (1) Get the current working directory and store it in new_cwd
 * (2) Retrieve the OLDPWD and PWD environment variables from the environment list.
 * (3) If PWD valid && current_cwd (current working directory) is valid, 
 * -> Create a new string in the format "PWD=<current_working_directory>".
 * -> Check for memory allocation failure or failure to add the variable to the environment.
 * -> Free the temporary string
 * (4) If OLDPWD valid && OLDPWD value exists/provided
 * -> Create a new string "OLDPWD=<previous_working_directory>". -> Check if fail -> Cleanup
 * 
*/
int update_pwd(t_shell *content, char *oldpwd_value)
{
	t_env	*old_pwd;
	t_env	*pwd;
	char	*current_cwd;
	char	*pwd_path;
	char	*oldpwd_path;

	current_cwd = getcwd(NULL, 0);
	
	old_pwd = get_env("OLDPWD", content->env);
	pwd = get_env("PWD", content->env);
	
	if (pwd && current_cwd)
	{
		pwd_path = ft_strjoin("PWD=", current_cwd);
		if (!pwd_path || add_envvar( pwd_path, &content->env) != 0)
			return (free(current_cwd), FAILURE);
		free (pwd_path);
	}
	if (old_pwd && oldpwd_value)
	{
		oldpwd_path = ft_strjoin("OLDPWD=", oldpwd_value);
		if (!oldpwd_path || add_envvar( oldpwd_path, &content->env) != 0)
			return (free(current_cwd), FAILURE);
		free(oldpwd_path);
	}
	free(current_cwd);
	return (SUCCESS);//0
}

/** ADD ENVVAR
 * Adds or updates an environment variable
 * Ensures that the variable's name and value are valid
 * Creates a new environment entry or updates an existing one.
 * 
 * @param
 * char	*arg_id;         // The name/ID of the environment variable.
 * char	*arg_value_dup;  // Duplicate of the value string.
 * t_env	*tmp;        // Temporary pointer to find or create the variable.
 * 
 * (1) Extract the variable name(id) from the input string. -> Check if valid
 * (2) Duplicate the entire "KEY=VALUE" string.
 * (3) Check if the variable already exists in the environment.
 * -> If fail OR env not exist => Create new one (initialize + add end-node)
 * -> If env exit => Update the existing variable.
 * 
*/
int	add_envvar(char *env_line, t_env **env_head)
{
	char	*env_id;
	char	*dup_envline;
	t_env	*env;
	
	env_id = get_env_id(env_line);
	if (!env_id || !check_envid_valid(env_id))
		return (free(env_id), error_export(env_line), FAILURE);

	dup_envline = ft_strdup(env_line);
	if (!dup_envline)
		return (free(env_id), FAILURE);//1

	env = get_env(env_line, *env_head);
	if (!env)
	{
		env = env_create(ft_strdup(env_id), ft_strdup(env_line), env_line);
		if (!env)
			return (free(env_line), free(env_id), 1);
		env_add_back(env_head, env);	
	}
	else
	{
		free (env_id);
		update_envvar(env, dup_envline);
	}
	return (SUCCESS);//0
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
			return (0);//invalid
		i++;
	}
	return (1);//env_id is valid
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

/* arg lstsize*/
int	ft_arg_lstsize(t_arg *args)
{
	int	i;

	i = 0;
	while (args)
	{
		i++;
		args = args->next;
	}
	return (i);
}