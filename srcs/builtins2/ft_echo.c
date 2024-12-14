/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:00:44 by yilin             #+#    #+#             */
/*   Updated: 2024/12/14 18:22:29 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"


/** ECHO (with option -n)
 * -n / -nnnnnn (followed only by character 'n') -> valid option => remove \n
 * -nP , -n-n, -nOPEK (followed by non 'n') -> invlaid
*/
int	ft_echo(t_arg *args)
{
	while (args && !check_echo_nflag(args->value)) //-n flag
		args = args->next; //skip -n flag
	while (args)
	{
		printf("%s", args->value);
		if (args->next)
			printf(" ");
		args = args->next;
	}
	// Print a newline if no -n flags were encountered
	if (args)// This will be true if there are any remaining arguments
		printf("\n");
	return (SUCCESS);
}

/** check echo nflag */
int	check_echo_nflag(char *flag)
{
	int	i;

	i = 0;
	if (!ft_strncmp(flag, "-n", 2))
		i += 2;
	else
		return (1); //NO, INVALID
	if (flag[i + 1] != 'n')
		return (1); //NO, INVALID
	return (0);
}