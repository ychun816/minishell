/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:37:19 by yilin             #+#    #+#             */
/*   Updated: 2024/12/15 17:50:18 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/** FT_ENV (with no options/args)
 * - If no argument -> print env (arrays of arrays)
 * - If there is argument -> output: env: '[input line]': No such file or directory
*/
int	ft_env(t_shell *content, t_arg *args) //t_shell *content, t_arg *args
{
	t_env	*env;
	char	*input_line;

	env = NULL;
	input_line = args->value;
	if (args)
	{
		error_env(input_line);
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
 * @note
 * *(args->value) == '-'
 * -> to check if the first character is '-' => valid option
 * -> Identify Flag Arguments
 * -> Prevent Invalid Arguments
 * (Without this check, you would attempt to compare a non-flag argument (like L) against "-L", which would produce incorrect results.)
*/
int	ft_pwd(t_arg *args)
{
	char	*cwd; //currunt working directory
	char	*option;

	option = args->value;
	if (args && *(args->value) == '-' && ft_strcmp(args->value, "-L") && ft_strcmp(args->value, "-P"))
	{
		error_pwd(option);
		return (2);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
    	perror("minishell: pwd: error retrieving current directory");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}