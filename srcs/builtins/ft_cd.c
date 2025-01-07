/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:56:29 by yilin             #+#    #+#             */
/*   Updated: 2025/01/07 11:38:39 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/** CD (relative or absolute path)
 * 1. Get the number of arguments passed to 'cd'
 * 2. If more than 1 args -> "error: too many arguments"
 * 3. Get the current working directory and store it in variable,
	if fail-> perror
 * 4. Get the 'HOME' environment variable
 * 5.
 **/
int	ft_cd(t_shell *content, t_arg *args)
{
	int		args_nb;
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
	if ((!args_nb || !ft_strcmp(args->value, "--")) && home && home->value)
		chdir(home->value);
	else if ((!args_nb || !ft_strcmp(args->value, "--")) && (!home
			|| !home->value))
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

	* Updates the PWD (current working directory) and OLDPWD (previous working directory) environment variables.
	*
 *
 * @param
 *
	- t_shell *content: A pointer to the shell's context structure containing the environment.
 * - t_env *old_pwd / pwd: interact wiht the environment
 * - char *current_cwd: Directly holds the raw directory path returned by getcwd
 * - char *pwd_path
 * - char *oldpwd_path
 *
 * (1) Get the current working directory and store it in new_cwd

	* (2) Retrieve the OLDPWD and PWD environment variables from the environment list.
 * (3) If PWD valid && current_cwd (current working directory) is valid,
 * -> Create a new string in the format "PWD=<current_working_directory>".
 *
	-> Check for memory allocation failure or failure to add the variable to the environment.
 * -> Free the temporary string
 * (4) If OLDPWD valid && OLDPWD value exists/provided
 * -> Create a new string "OLDPWD=<previous_working_directory>".
	-> Check if fail -> Cleanup
 *
 */
int	update_pwd(t_shell *content, char *oldpwd_value)
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
		if (!pwd_path || add_envvar(pwd_path, &content->env) != 0)
			return (free(current_cwd), FAILURE);
		free(pwd_path);
	}
	if (old_pwd && oldpwd_value)
	{
		oldpwd_path = ft_strjoin("OLDPWD=", oldpwd_value);
		if (!oldpwd_path || add_envvar(oldpwd_path, &content->env) != 0)
			return (free(current_cwd), FAILURE);
		free(oldpwd_path);
	}
	free(current_cwd);
	return (SUCCESS);
}

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
