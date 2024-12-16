/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:53:59 by yilin             #+#    #+#             */
/*   Updated: 2024/12/16 20:07:33 by yilin            ###   ########.fr       */
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
	if (!args)
		export_print_ordered_env(content->env);
	else
	{
		while (args)
		{
			if (add_envvar(args->value, &(content->env)) != 0)
			{
				printf("🎅🎅🎅🎅🎅 add envvar FAIL CON 🎅🎅🎅🎅🎅\n");
				return (FAILURE);//1
			}
			printf("🎅🎅🎅🎅🎅 add envvar HERE? 🎅🎅🎅🎅🎅\n");
			args = args->next;
		}
	}
	return (SUCCESS);
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
void	export_print_ordered_env(t_env *env)
{
	char	**env_arrs;
	char	**sorted;
	char	*tmp_value;
	char	*tmp_id;
	int	i;

	i = 0;
	env_arrs = env_format(env);
	if (!env_arrs)
		return ;
	sorted = sort_env_arrs(env_arrs);
	if (!sorted)
		return ;
	while (sorted[i])
	{
		printf("export ");
		tmp_value = ft_strchr(sorted[i], '=') + 1;
		if (!tmp_value)
			printf ("%s\n", sorted[i]);
		else
		{
			tmp_id = get_env_id(sorted[i]);
			printf("%s = %s\n", tmp_id, tmp_value);
			free(tmp_id);
		}
		i++;
	}
	free(env_arrs);
    free(sorted);
}

/** sort_env_arrs */
char **sort_env_arrs(char **env_arrs)
{
	char	**result;
	int	i;
	int	j;
	int	len;
	char	*swap;
	
	len = 0;
	while (env_arrs[len])
		len++;
	// result = malloc(sizeof(char*) * (len));
	result = malloc(sizeof(char*) * (len + 1));//save space for NULL
	if (!result)
		return (NULL);

	//copy env_arrs to reuslt
	i = 0;
	while (i < len)
	{
		result[i] = ft_strdup(env_arrs[i]);
		i++;
	}
	result[i] = NULL;

	//sorting		
	i = 0;
	while(result[i])
	{
		j = i + 1;
		while (result[j])
		{
			if (ft_strcmp(result[i], result[j]) > 0)
			{
				swap = result[i];
				result[i] = result[j];
				result[j] = swap;
			}
			j++;
		}
		i++;
	}
	//free env_vars?
	return (result);
}