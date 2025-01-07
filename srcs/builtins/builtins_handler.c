/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:04:26 by yilin             #+#    #+#             */
/*   Updated: 2025/01/07 16:54:54 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/** 42 subject for builtins:
 * - pwd with no options
 * - export with no options
 * - unset with no options
 * - env with no options or arguments
 * - exit with no options
 * - echo with option -n
 * - cd with only a relative or absolute path
 */

/** CHECK IS BUILTIN **/
int	check_is_builtin(char *input_cmd)
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
int	exec_builtin(t_shell *content, char *input_cmd, t_arg *args)
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
