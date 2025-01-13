/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_error1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:26:27 by yilin             #+#    #+#             */
/*   Updated: 2025/01/07 18:12:36 by yilin            ###   ########.fr       */
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
void	error_pwd(char *option)
{
	ft_putstr_fd(P_NAME, STDERR_FILENO);
	ft_putstr_fd(" pwd: ", STDERR_FILENO);
	ft_putstr_fd(option, STDERR_FILENO);
	ft_putstr_fd(": Invalid option\n", STDERR_FILENO);
}

/** ERROR EXIT */
void	error_exit(char *input_line)
{
	ft_putstr_fd(P_NAME, STDERR_FILENO);
	ft_putstr_fd(": exit: ", STDERR_FILENO);
	ft_putstr_fd(input_line, STDERR_FILENO);
	ft_putstr_fd(": Numeric argument required\n", STDERR_FILENO);
}
