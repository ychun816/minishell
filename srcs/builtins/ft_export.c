/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:53:59 by yilin             #+#    #+#             */
/*   Updated: 2024/12/30 16:32:21 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/** EXPORT (with no options)
 * Handling casess: 
 * 1. export
 * 2. export ENV_VARIABLE=VALUE (-> add env variable)
 * 3. export ENV_VARIABLE1=VALUE1 ENV_VARIABLE2=VALUE2 ENV_VARIABLE3=VALUE3
 * 
 * - If no arg -> print env according to alphabetic order
 * - If there's arg
*/
int	ft_export(t_shell *content, t_arg *args)
{	
	int	exit_code;

	exit_code = 0;
	if (!args)
	{
		if (export_print_ordered_env(content->env)!= 0)
		{
			//exit_code = 1;
			return (SUCCESS);
		}
	}
	else
	{
		while (args)
		{
			if (add_envvar(args->value, &(content->env)) != 0)
				return (FAILURE);
				//exit_code = 1;
			args = args->next;
		}
	}
	// return (exit_code);
	return (SUCCESS);
}

static void	print_all_export(char **sorted)
{
	int	i;
	char	*tmp_value;
	char	*tmp_id;

	i = -1;
	while (sorted[++i])
	{
		printf("export ");
		tmp_value = ft_strchr(sorted[i], '=');
		if (!tmp_value)
			printf ("%s\n", sorted[i]);
		else
		{
			tmp_value += 1;
			tmp_id = get_env_id(sorted[i]);
			printf("%s=\"%s\"\n", tmp_id, tmp_value);
			free(tmp_id);
		}
	}
}

/** export_print_ordered_env 
 * (1) Transform env list to env arrays
 * (2) Sort env arrays by alphabetic orders
 * (3) Print arrays (export USER="lin"), 'export' + 'VAR=VALUE'
 * (4) Free the original environment array
 * (5) Free the sorted array
 * @note tmp_value = ft_strchr(sorted[i], '=') + 1; -> +1 to get value after '='
 * 
*/
int	export_print_ordered_env(t_env *env)
{
	char	**env_arrs;
	char	**sorted;

	env_arrs = env_format(env);
	if (!env_arrs)
		return (FAILURE);
	sorted = sort_env_arrs(env_arrs);
	if (!sorted)
		return (arrs_free(env_arrs), FAILURE);
	print_all_export(sorted);
	arrs_free(sorted);
	arrs_free(env_arrs);
	return (SUCCESS);
}

static void	ft_swap_ptr(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;	
}

/** sort_env_arrs
 * (1) Count the len of arrs
 * (2) copy env_arrs to reuslt, ft_strdup unsorted arrs
 * (3) sort arrs 
*/
char **sort_env_arrs(char **env_arrs)
{
	char	**result;
	int	i;
	int	j;

	result = ft_arrsdup(env_arrs);
	if (!result)
		return (arrs_free(result), NULL);
	i = -1;
	while (result[++i])
	{
		j = i + 1;
		while (result[j])
		{
			if (ft_strcmp(result[i], result[j]) > 0)
				ft_swap_ptr(&result[i], &result[j]);
			j++;
		}
	}
	return (result);
}
