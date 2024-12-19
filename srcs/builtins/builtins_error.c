/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:26:27 by yilin             #+#    #+#             */
/*   Updated: 2024/12/14 18:24:26 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/** ERROR ENV */
void	error_env(char *input_line)
{
	ft_putstr_fd("env: ", STDERR_FILENO);
	ft_putstr_fd(input_line, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
}

/** ERROR PWD */
// fprintf(STDERR_FILENO, "%s: pwd: %s: invalid option\n", P_NAME, option);
void	error_pwd(char	*option)
{
	ft_putstr_fd(P_NAME, STDERR_FILENO);
	ft_putstr_fd(" pwd: ", STDERR_FILENO);
	ft_putstr_fd(option, STDERR_FILENO);
	ft_putstr_fd(": invalid option\n", STDERR_FILENO);
}

/** ERROR EXIT */
//printf("%s: exit: %s: numeric argument required\n", P_NAME, val);
void	error_exit(char *input_line)
{
	ft_putstr_fd(P_NAME, STDERR_FILENO);
	ft_putstr_fd(": exit: ", STDERR_FILENO);
	ft_putstr_fd(input_line, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
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
		ft_putstr_fd(P_NAME, STDERR_FILENO);
		ft_putstr_fd(": cd: ", STDERR_FILENO);
		ft_putstr_fd(pathname, STDERR_FILENO);
		ft_putstr_fd(" not set\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd(P_NAME, STDERR_FILENO);
		ft_putstr_fd(": cd: ", STDERR_FILENO);
		ft_putstr_fd(pathname, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(err_msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
}

/** ERROR EXPORT */
//printf("%s: export: %s: not a valid identifier\n", P_NAME, var);
void	error_export(char *env_line)
{
	ft_putstr_fd(P_NAME, STDERR_FILENO);
	ft_putstr_fd(": export: ", STDERR_FILENO);
	ft_putstr_fd(env_line, STDERR_FILENO);
	ft_putstr_fd(" : not a valid identifier\n", STDERR_FILENO);
}