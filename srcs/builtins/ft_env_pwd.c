/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:37:19 by yilin             #+#    #+#             */
/*   Updated: 2025/01/07 11:40:08 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/** FT_ENV (with no options/args)
 * - If no argument -> print env (arrays of arrays)
 * - If there is argument
	-> output: env: '[input line]': No such file or directory
 */
/* OG
int	ft_env(t_shell *content, t_arg *args)
{
	t_env	*env;
	t_env	*env;

	env = content->env;
	if (!env)
	{
		if (env->next)
			env = env->next;
	}
	if (args)
	{
		error_env(args->value);
		content->exit_code = CMD_NOT_FOUND;
	}
	while (env->next)
	{
		if (env->value)
			printf("%s\n",env->env_line);
		env = env->next;
	}
	if (env && env->next == NULL)
		printf("%s\n",env->env_line);
	return (SUCCESS);
}*/
/* modified trying */
int	ft_env(t_shell *content, t_arg *args)
{
	t_env	*env;

	env = content->env;
	if (args)
	{
		error_env(args->value);
		content->exit_code = CMD_NOT_FOUND;
		// return (FAILURE);
	}
	// Check if env list exists
	if (!env)
		return (SUCCESS); // Empty environment is valid
	// Loop through all environment variables
	while (env)
	{
		// Only print if variable has a value
		if (env->value)
			printf("%s\n", env->env_line);
		env = env->next;
	}
	// if (env && env->next == NULL)
	// 	printf("%s\n",env->env_line);
	return (SUCCESS);
}

/** PWD (no options)
 * - If no argument
	-> absolute path name of the current directory that does not contain the file names
 * - pwd -L / pwd -P
 * @param cwd: current working directory
 * @note
 * *(args->value) == '-'
 * -> to check if the first character is '-' => valid option
 * -> Identify Flag Arguments
 * -> Prevent Invalid Arguments: args->value != "-L" && args->value != "-P"
 * (Without this check,
	you would attempt to compare a non-flag argument (like L) against "-L",
	which would produce incorrect results.)
 */
int	ft_pwd(t_arg *args)
{
	char	*cwd;

	if (args && *(args->value) == '-' && ft_strcmp(args->value, "-L") != 0
		&& ft_strcmp(args->value, "-P") != 0)
	{
		error_pwd(args->value);
		return (FAILURE_VOID);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: pwd: error retrieving current directory");
		return (FAILURE);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (SUCCESS);
}
