/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:26:27 by yilin             #+#    #+#             */
/*   Updated: 2024/12/12 14:54:06 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** ERROR ENV */
void	error_env(char *input_line)
{
	ft_putstr_fd("env: ", stderr);
	ft_putstr_fd(input_line, stderr);
	ft_putstr_fd(": No such file or directory\n", stderr);
}

/** ERROR PWD */
// fprintf(stderr, "%s: pwd: %s: invalid option\n", P_NAME, option);
void	error_pwd(char	*option)
{
	ft_putstr_fd(P_NAME, stderr);
	ft_putstr_fd(" pwd: ", stderr);
	ft_putstr_fd(option, stderr);
	ft_putstr_fd(": invalid option\n", stderr);
}

/** ERROR EXIT */
//printf("%s: exit: %s: numeric argument required\n", P_NAME, val);
void	error_exit(char *input_line)
{
	ft_putstr_fd(P_NAME, stderr);
	ft_putstr_fd(": exit: ", stderr);
	ft_putstr_fd(input_line, stderr);
	ft_putstr_fd(": numeric argument required\n", stderr);
}

/** ERROR CD */
//printf("%s: cd: %s not set\n", P_NAME, file);
//printf("%s: cd: %s: %s\n", P_NAME, file, strerror(err_no));
void	error_cd(int err_n, char *pathname)
{
	char	*err_msg;

	err_msg = strerror(err_n);
	if (pathname && ft_strcmp(pathname, "HOME") == 0)
	{
		ft_putstr_fd(P_NAME, stderr);
		ft_putstr_fd(": cd: ", stderr);
		ft_putstr_fd(pathname, stderr);
		ft_putstr_fd(" not set\n", stderr);
	}
	else
	{
		ft_putstr_fd(P_NAME, stderr);
		ft_putstr_fd(": cd: ", stderr);
		ft_putstr_fd(pathname, stderr);
		ft_putstr_fd(": ", stderr);
		ft_putstr_fd(err_msg, stderr);
		ft_putstr_fd("\n", stderr);
	}
}

