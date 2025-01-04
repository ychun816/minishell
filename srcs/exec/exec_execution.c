/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:12:01 by varodrig          #+#    #+#             */
/*   Updated: 2024/12/30 18:55:53 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* OG find_path
// access() returns 0 if OK
char	*find_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*path;
	char	*exec;
	t_env	*curr;
	int		i;

	curr = env;
	//printf("🤡curr->env_line: %s\n", curr->env_line);
	while (curr && ft_strncmp("PATH=", curr->env_line, 5) != 0)
		curr = curr->next;
	//printf("🤡curr: %s\n", curr->value);
	//printf("🤡curr->next: %s\n", curr->next->value);
	if (!curr)
		return (ft_strdup(cmd));
	paths = ft_split(curr->value, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		exec = ft_strjoin(path, cmd);
		//printf("🤡path: %s\n", path);
		//printf("🤡exec: %s\n", exec);
		free(path);
		if (!access(exec, X_OK | F_OK))
		{
			ft_free_allcurr = env; path */
static int	check_is_abs_path(char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '/')
			return (1);
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
    if (!env || check_is_abs_path(cmd) == 1)
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
/*
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
			arrs_free(env);
			return (4);
		}
		if (execve(path, args, env) == -1)
		{
			err_execve(path, errno);
			free(path);
			arrs_free(env);//og: free(env);
			return (-2);
		}
		free(path);
	}
	free(env);//arrs_free(env)?
	return (0);
}
*/

/* DEBUG TRY V2 */
int ft_execution(t_shell *ctx, t_exec *temp)
{
    int args_nb;
    char *path;
    char *args[ft_args_lstsize(temp->args) + 2];
    char **env;

    if (!temp->cmd)
        return (0);
    // Get path first - no direct execution attempt
    path = find_path(temp->cmd, ctx->env);
    if (!path)
    {
        err_execve(temp->cmd, errno);
        return (4);
    }
    // Create environment and args only if path is valid
    env = env_format(ctx->env);
    if (!env)
    {
        free(path);
        return (4);
    }

    args_nb = ft_args_lstsize(temp->args) + 2;
    exec_args_create(temp, args_nb, args);

    // Single execve attempt with found path
    if (execve(path, args, env) == -1)
    {
        err_execve(path, errno);
        free(path);
        arrs_free(env);
        return (-2);
    }
    free(path);
    arrs_free(env);
    return (0);
}
