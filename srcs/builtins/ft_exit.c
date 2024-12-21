/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:00:20 by yilin             #+#    #+#             */
/*   Updated: 2024/12/18 17:53:02 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/** EXIT (no options)
 * exit [n]
 * - If more than 1 argument -> error: too many arguments
 * - if 1 argument & is a valid exit code (it should be a number)
	-> exit with exit code
 * - If 1 argument & is invalid -> error_exit()
 * In the end, close and free everything
 */
int	ft_exit(t_shell *content, t_arg *args)
{
	int	exit_code;

	if (args && args->next && !check_exitcode(args->value))
		return (ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO), 1);
	exit_code = 0;
	if (args && !check_exitcode(args->value))
		exit_code = ft_atoi(args->value);
	else if (args && check_exitcode(args->value))
	{
		error_exit(args->value);
		exit_code = 2;
	}
	set_std(content, 1);
	free_all_shell(content);
	exit(exit_code);
}

/** CHECK_EXIT_CODE
 *
 * @note
 * char *char tmp : point to the provided value
	-> Preserving the Original Pointer
 * If the value starts with '+' or '-', skip the sign
 * Iterate through each character in the value to check if it's a digit
 * Convert the value to a long integer using ft_atol
 * Check if the number overflows or underflows the range of valid exit codes
 * Return 0 if the value is a valid exit code
 */
int	check_exitcode(char *input_line)
{
	char *tmp;
	long exit_nb;

	tmp = input_line;
	if (((*tmp) == '+' || (*tmp) == '-') && *(tmp + 1))
		tmp++;
	while (*tmp)
	{
		if (!ft_isdigit(*tmp))
			return (1);
		tmp++;
	}
	exit_nb = ft_atol(input_line);
	if ((exit_nb > 0 && LONG_MAX / exit_nb < 1) || (exit_nb < 0 && LONG_MIN
			/ ft_atol(input_line) < 1))
		return (1);
	return (0);
}
