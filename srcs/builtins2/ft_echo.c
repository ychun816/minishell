/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:00:44 by yilin             #+#    #+#             */
/*   Updated: 2024/12/16 15:44:58 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/** FT_ECHO (with option -n)
 * -n / -nnnnnn (followed only by character 'n') -> valid option => remove \n
 * -nP , -n-n, -nOPEK (followed by non 'n') -> invlaid
 * @note use n_arg to flag when 'n' is found
*/
int	ft_echo(t_arg *args)
{
	int	n_arg;
	
	n_arg = 0;
	while (args && check_echo_has_nargs(args->value) != 0)
	{
		n_arg = 1;
		args = args->next;
	}
	while (args)
	{
		printf("%s", args->value);
		if (args->next)
			printf(" ");
		args = args->next;
	}
	if (n_arg == 0)
		printf("\n");
	return (SUCCESS);
}

/** CHECK ECHO HAS N ARGS 
 * - return (0) if no n found
 * - return (1) if n found -> valid n args
 * @note Ensure has '-' in first character, then check rest
*/
int	check_echo_has_nargs(char *flag)
{
	int	i;

	i = 0;
	if (!flag || flag[0] != '-')
		return (0);
	i += 1;
	while (flag[i])
	{
		if (flag[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}