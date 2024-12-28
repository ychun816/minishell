/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:12:01 by varodrig          #+#    #+#             */
/*   Updated: 2024/12/28 19:27:04 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// access() returns 0 if OK
char	*find_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*path;
	char	*exec;
	t_env	*curr;
	int		i;

	curr = env;
	while (curr && ft_strncmp("PATH=", curr->env_line, 5) != 0)
		curr = curr->next;
	if (!curr)
		return (ft_strdup(cmd));
	paths = ft_split(curr->value, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		exec = ft_strjoin(path, cmd);
		free(path);
		if (!access(exec, X_OK | F_OK))
		{
			ft_free_all(paths);
			return (exec);
		}
		free(exec);
		i++;
	}
	ft_free_all(paths);
	return (NULL);
}

int	ft_env_lstsize(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**env_format(t_env *env)
{
	char	**env_arr;
	int		size;
	int		i;

	size = ft_env_lstsize(env);
	env_arr = (char **)malloc(sizeof(char *) * (size + 1));
	if (!env_arr)//env
		return (NULL);
	i = 0;
	while (i < size && env)
	{
		env_arr[i] = ft_strdup(env->env_line);
		// env_arr[i] = env->env_line;
		if (!env_arr[i])
		{
			arrs_free(env_arr);// Free already-allocated memory if duplication fails
			return (NULL);
		}
		env = env->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

// 126 : Command found but cannot be executed(due to permission issues)
// 127 : Command not found(the file does not exist or is not in the PATH)
// execve(path, comd, env);
// char	*args[] = {"/bin/ls", "-l", "/home", NULL};
int	ft_execution(t_shell *ctx, t_exec *temp)
{
	int		args_nb;
	char	*path;
	char	*args[ft_args_lstsize(temp->args) + 2];
	char	**env;

	args_nb = ft_args_lstsize(temp->args) + 2;
	exec_args_create(temp, args_nb, args);
	env = env_format(ctx->env);
	if (!env)
		return (4);
	if (execve(temp->cmd, args, env) == -1)
	{
		path = find_path(temp->cmd, ctx->env);
		if (!path)
		{
			err_execve(temp->cmd, errno);
			free(env);
			return (4);
		}
		if (execve(path, args, env) == -1)
		{
			err_execve(path, errno);
			free(path);
			free(env);
			return (-2);
		}
		free(path);
	}
	free(env);
	return (0);
}
