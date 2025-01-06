/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:12:01 by varodrig          #+#    #+#             */
/*   Updated: 2025/01/06 15:42:35 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_is_alr_path(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

/** Modified find_path
 * + Add empty string check: check both condition -> avoid segfault
 * + Add Add absolute path check! MUST check before search in PATH!
 * @note
 * Absolute paths should always be checked first!
 * -> cuz they provide the full location of the executable
 * -> and can be used directly without needing to search in PATH!
 */
char	*find_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*path;
	char	*exec;
	t_env	*curr;
	int		i;

	if (cmd && cmd[0] == '\0')
		return (NULL);
	if (!env || check_is_alr_path(cmd) == 1)
		return (ft_strdup(cmd));
	curr = env;
	while (curr && ft_strncmp("PATH=", curr->env_line, 5) != 0)
		curr = curr->next;
	if (!curr)
		return (ft_strdup(cmd));
	paths = ft_split(curr->value, ':');
	i = -1;
	while (paths[++i])
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
	if (!env_arr)
		return (NULL);
	i = 0;
	while (i < size && env)
	{
		env_arr[i] = ft_strdup(env->env_line);
		if (!env_arr[i])
		{
			arrs_free(env_arr);
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
// 
int	ft_execution(int args_size, t_shell *ctx, t_exec *temp)
{
	int		args_nb;
	char	*path;
	char	*args[args_size + 2];
	char	**env;

	if (!temp->cmd)
		return (0);
	path = find_path(temp->cmd, ctx->env);
	if (!path)
	{
		err_execve(temp->cmd, errno);
		return (4);
	}
	env = env_format(ctx->env);
	if (!env)
	{
		free(path);
		return (4);
	}
	args_nb = ft_args_lstsize(temp->args) + 2;
	exec_args_create(temp, args_nb, args);
	if (execve(path, args, env) == -1)
	{
		err_execve(path, errno);
		free(path);
		ft_free_all(env);
		return (-2);
	}
	free(path);
	ft_free_all(env);
	return (0);
}
