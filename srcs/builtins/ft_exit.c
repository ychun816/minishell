/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:00:20 by yilin             #+#    #+#             */
/*   Updated: 2025/01/08 15:11:41 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** FT_EXIT (no options)
 * exit [n]
 * (1) If more than 1 argument -> error: too many arguments
 * (2) if 1 argument & is a valid exit code (it should be a number)
 * -> exit with exit code
 * (3) If 1 argument & is invalid -> error_exit()
 * (4) Close and free everything
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
 * @param *char tmp : point to the provided value
 * -> Preserving the Original Pointer
 * (1) If the value starts with '+' or '-' -> skip the sign
 * (2) Iterate through each character in the value to check if it's a digit
 * (3) Convert the value to a long integer with ft_atol()
 * (4) Check if the number overflows OR underflows the range of valid exit_code
 * @note 2-step casting!!
 * exit_status = (unsigned char *)exit_nb;
 * -> Ensures the value is constrained to 8 bits (0–255).
 * (long)exit_status != exit_nb;
 * -> Compare the modified value with og value to detect discrepancies.
 * @return (0) if is a valid exit code
 */
int	check_exitcode(char *input_line)
{
	char			*tmp;
	long			exit_nb;
	unsigned char	*exit_status;

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
	exit_status = (unsigned char *)exit_nb;
	if ((long)exit_status != exit_nb)
		return (1);
	return (0);
}
