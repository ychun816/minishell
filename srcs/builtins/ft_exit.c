/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:00:20 by yilin             #+#    #+#             */
/*   Updated: 2025/01/09 20:19:06 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_atol_overflow(const char *str, long *result);

/** FT_EXIT (no options)
 * exit [n]
 * (1) If more than 1 argument -> error: too many arguments
 * (2) if 1 argument & is a valid exit code (it should be a number)
 * -> exit with exit code
 * (3) If 1 argument & is invalid -> error_exit()
 * (4) Close and free everything
 * @note
 * //Apply modulo 256 to match Bash behavior!!!
 */
int	ft_exit(t_shell *content, t_arg *args)
{
	long	exit_code;

	exit_code = content->exit_code;
	if (args)
	{
		if (check_exitcode(args->value) || ft_atol_overflow(args->value,
				&exit_code))
		{
			error_exit(args->value);
			exit_code = 2;
		}
		else if (args->next)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO);
			exit_code = 1;
		}
		else
			exit_code = (unsigned char)(exit_code % 256);
	}
	set_std(content, 1);
	free_all_shell(content);
	exit(exit_code);
}

/** FT_ATOL / CHECK OVERFLOW
 * @param long boundary;
 * // Store the limit for the overflow check
 * @param long digit_value = digit - '0';
 * // Convert char to int value
 * (1) Determine the boundary depending on the sign
 * (positive or negative)
 * (2) For positive numbers, use LONG_MAX
 * -> Check for overflow for positive numbers
 * (3) For negative numbers, use LONG_MIN
 * -> Check for overflow for negative numbers
 * @return (1) overflow detected / (0) no overflow
 */
static int	check_overflow(long res, int sign, char digit)
{
	long	boundary;
	long	digit_value;

	digit_value = digit - '0';
	if (sign == 1)
	{
		boundary = LONG_MAX / 10;
		if (res > boundary || (res == boundary && digit_value > LONG_MAX % 10))
			return (1);
	}
	else
	{
		boundary = -(LONG_MIN / 10);
		if (res > boundary || (res == boundary && digit_value > -(LONG_MIN
					% 10)))
			return (1);
	}
	return (0);
}

static int	ft_atol_overflow(const char *str, long *result)
{
	long	res;
	int		sign;

	res = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		else
			sign = 1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (check_overflow(res, sign, *str) == 1)
			return (1);
		res = res * 10 + (*str - '0');
		str++;
	}
	*result = res * sign;
	return (0);
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
 * -> Ensures ft_exit.c: Errorthe value is constrained to 8 bits (0–255).
 * (long)exit_status != exit_nb;
 * -> Compare the modified value with og value to detect discrepancies.
 * @return (0) if is a valid exit code
 */
int	check_exitcode(char *input_line)
{
	char	*tmp;

	tmp = input_line;
	if (!input_line || !*input_line)
		return (1);
	while (*tmp == ' ' || (*tmp >= 9 && *tmp <= 13))
		tmp++;
	if (*tmp == '\0')
		return (1);
	if ((*tmp == '+' || *tmp == '-') && *(tmp + 1))
		tmp++;
	while (*tmp)
	{
		if (!isdigit(*tmp))
			return (1);
		tmp++;
	}
	return (0);
}

/*OG
(fixed too long)
int	ft_exit(t_shell *content, t_arg *args)
{
	long	exit_code;
	int		overflow;

	exit_code = content->exit_code;
	if (!args)
	{
		set_std(content, 1);
		free_all_shell(content);
		exit(exit_code);
	}
	if (check_exitcode(args->value))
	{
		error_exit(args->value);
		set_std(content, 1);
		free_all_shell(content);
		exit(2);
	}
	overflow = ft_atol_overflow(args->value, &exit_code);
	if (overflow)
	{
		error_exit(args->value);
		set_std(content, 1);
		free_all_shell(content);
		exit(2);
	}
	if (args->next)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		exit(1);
	}
	exit_code = (unsigned char)(exit_code % 256);
	set_std(content, 1);
	free_all_shell(content);
	exit(exit_code);
}
///(wrong)
int	ft_exit(t_shell *content, t_arg *args)
{
	int		exit_code;
	long	res;
	int		sign;

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
}*/
/* OG
// static int check_overflow(long res, int sign, char digit)
// {
//     if ((sign == 1 && (res > LONG_MAX / 10 ||
//         (res == LONG_MAX / 10 && (digit - '0') > LONG_MAX % 10))) ||
//         (sign == -1 && (res > -(LONG_MIN / 10) ||
//         (res == -(LONG_MIN / 10) && (digit - '0') > -(LONG_MIN % 10)))))
//         return (1);
//     return (0);
// }
//// (long vr.)
static int	ft_atol_overflow(const char *str, long *result)
{
	res = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if ((sign == 1 && (res > LONG_MAX / 10 ||
			(res == LONG_MAX / 10 && (*str - '0') > LONG_MAX % 10))) ||
			(sign == -1 && (res > -(LONG_MIN / 10) ||
			(res == -(LONG_MIN / 10) && (*str - '0') > -(LONG_MIN % 10)))))
		{
			return (1);// Overflow detected
		}
		res = res * 10 + (*str - '0');
		str++;
	}
	*result = res * sign;
	return (0);// No overflow
}
*/
/*
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
		if (!ft_isdigit(*tmp++))
			return (1);
	}
	exit_nb = ft_atol(input_line);
	exit_status = (unsigned char *)exit_nb;
	if ((long)exit_status != exit_nb)
		return (1);
	return (0);
}*/
