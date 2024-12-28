/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrs_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:52:20 by yilin             #+#    #+#             */
/*   Updated: 2024/12/28 18:32:15 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** @note
 * - Loop to free each array
 * - Free pointer itself in the end
 */
void	arrs_free(char **arrs)
{
	int	i;

	// if (!arrs)
    //     return ;
	i = 0;
	while (arrs[i])
	{
		free(arrs[i]);
		i++;
	}
	free(arrs);
}