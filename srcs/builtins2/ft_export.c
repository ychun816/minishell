/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:53:59 by yilin             #+#    #+#             */
/*   Updated: 2024/12/14 18:27:11 by yilin            ###   ########.fr       */
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
	{
		export_print_ordered_env(content->env);
		return (FAILURE);//1
	}
	else
	{
		while (args)
		{
			if (add_envvar(args->value, &(content->env)) != 0)
				return (FAILURE);//1
			args = args->next;
		}
	}
	return (SUCCESS);
}

/** export_print_ordered_env 
 * (1) transform env list to env arrays
 * (2) sort env arrays by alphabetic orders
 * (3) print arrays
 * 
*/
void	export_print_ordered_env(t_env *env)
{
	char	**env_arrs;
	char	**sorted_envarrs;
	char	*tmp_value;
	char	*tmp_id;
	int	i;

	i = 0;
	env_arrs = env_format(env);
	if (!env_arrs)
		return ;
	sorted_envarrs = sort_env_arrs(env_arrs);
	
	//print env arrs  //export USER="lin"
	while (sorted_envarrs[i])
	{
		printf("export ");//print export
		//get env_var
		tmp_value = ft_strchr(sorted_envarrs[i], '=') + 1;
		if (!tmp_value)
			printf ("%s\n", sorted_envarrs[i]);
		else
		{
			tmp_id = get_env_id(sorted_envarrs[i]);
			printf("%s = %s\n", tmp_id, tmp_value);
			free(tmp_id);
		}
		free(sorted_envarrs);	
		i++;
	}
	free(env_arrs);  // Free the original environment array
    free(sorted_envarrs);  // Free the sorted array
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
		j = 1;
		while (result[i + j])
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
	return (result);
}