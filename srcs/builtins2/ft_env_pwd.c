/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:37:19 by yilin             #+#    #+#             */
/*   Updated: 2024/12/16 17:44:08 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/** FT_ENV (with no options/args)
 * - If no argument -> print env (arrays of arrays)
 * - If there is argument -> output: env: '[input line]': No such file or directory
*/
int	ft_env(t_shell *content, t_arg *args)
{
	t_env	*env;

	env = content->env;
	if (args)
	{
		error_env(args->value);
		content->exit_code = CMD_NOT_FOUND;//comment not found=127
	}
	while (env)
	{
		if (env->value)
			printf("%s\n",env->env_line);
		env = env->next;
	}
	return (SUCCESS);
}

/** PWD (no options)
 * - If no argument -> absolute path name of the current directory that does not contain the file names
 * - pwd -L / pwd -P
 * @param cwd: current working directory
 * @note
 * *(args->value) == '-'
 * -> to check if the first character is '-' => valid option
 * -> Identify Flag Arguments
 * -> Prevent Invalid Arguments: args->value != "-L" && args->value != "-P"
 * (Without this check, you would attempt to compare a non-flag argument (like L) against "-L", which would produce incorrect results.)
*/
int	ft_pwd(t_arg *args)
{
	char	*cwd;

	if (args && *(args->value) == '-' && ft_strcmp(args->value, "-L") != 0 && ft_strcmp(args->value, "-P") != 0)
	{
		error_pwd(args->value);
		return (FAILURE_VOID);//2: Misuse of a command or syntax error (invalid option or argument)
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
    	perror("minishell: pwd: error retrieving current directory");
		return (FAILURE);//1
	}
	printf("%s\n", cwd);
	free(cwd);
	return (SUCCESS);//0	
}