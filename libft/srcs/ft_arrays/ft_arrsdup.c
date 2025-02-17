/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrsdup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:48:56 by yilin             #+#    #+#             */
/*   Updated: 2024/12/28 18:30:51 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @note
 * + result = malloc(sizeof(char *) * (srcslen + 1));
 * Save space for NULL
 * + arrs_free(result);
 * Free everything allocated so far
 */
char	**ft_arrsdup(char **src_arrs)
{
	char	**result;
	int		srcslen;
	int		i;

	srcslen = ft_arrslen(src_arrs);
	result = malloc(sizeof(char *) * (srcslen + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (src_arrs[i])
	{
		result[i] = ft_strdup(src_arrs[i]);
		if (!result[i])
		{
			arrs_free(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}
