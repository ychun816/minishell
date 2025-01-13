/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:00:44 by yilin             #+#    #+#             */
/*   Updated: 2025/01/09 22:30:53 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	check_n_option(char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(t_arg *args)
{
	int	n_flag;

	n_flag = 0;
	while (args && args->value && check_n_option(args->value))
	{
		n_flag = 1;
		args = args->next;
	}
	while (args)
	{
		ft_printf("%s", args->value);
		if (args->next)
			ft_printf(" ");
		args = args->next;
	}
	if (!n_flag)
		ft_printf("\n");
	return (SUCCESS);
}
