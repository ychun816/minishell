/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:37:19 by yilin             #+#    #+#             */
/*   Updated: 2024/12/09 19:47:35 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** CHECK IS BUILTIN (bi_is_builtin)
 * builtin: echo / cd / pwd / export / unset / env / exit
*/
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

/** EXEC BUILTIN (bi_is_builtin)
 * builtin: echo / cd / pwd / export / unset / env / exit
*/
int	exec_builtin(t_shell *content, char* input_cmd, t_arg *args)
{
	if (ft_strcmp(input_cmd, "echo") == 0)
		return (builtin_echo(args));
	else if (ft_strcmp(input_cmd, "cd") == 0)
		return (builtin_cd(content, args));
	else if (ft_strcmp(input_cmd, "pwd") == 0)
		return (builtin_pwd(content));
	else if (ft_strcmp(input_cmd, "export") == 0)
		return (builtin_export(content, args));
	else if (ft_strcmp(input_cmd, "unset") == 0)
		return (builtin_unset(content, args));
	else if (ft_strcmp(input_cmd, "env") == 0)
		return (builtin_env(content, args));
	else if (ft_strcmp(input_cmd, "exit") == 0)
		return (builtin_exit(content, args));
	else
		return (0);
}

int	builtin_echo(t_arg *args);
int	builtin_cd();
int	builtin_pwd();
int	builtin_export();
int	builtin_unset();
int	builtin_env();
int	builtin_exit();

