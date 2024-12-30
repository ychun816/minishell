/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:14:21 by yilin             #+#    #+#             */
/*   Updated: 2024/12/30 21:02:03 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// FT_ENVVAR_LEN
//  * - Counts the length of "USER" AFTER $.
//  * - Starts from the given string `env_var`
//  * - Checks for valid characters that can make up an
//		environment variable's name.
//  * @note
//  * - A valid name can contain letters, digits, and underscores.
//  * - If the first character is a digit, '?' or '$', it's invalid
//  * 		as an environment variable name.
//  * - If a character is neither alphanumeric nor an underscore, stop counting.
//  * @return (int) length AFTER $ sign
int	ft_envvar_len(char	*env_var)
{
	int	i;

	i = 0;
	if (env_var[i] == '$')
		i++;
	if (ft_isdigit(env_var[i]) == 1 || env_var[i] == '?' || env_var[i] == '$')
		return (2);
	while (env_var[i])
	{
		if (env_var[i] != '_' && ft_isalnum(env_var[i]) == 0)
			break ;
		i++;
	}
	return (i);
}

/** GET ENVVAR NAME
 * Extracts the variable name "USER" from "$USER".
 * @return (char *)
 */
char	*get_envvar_name(char *env_var)
{
	int		len;
	char	*name;

	len = ft_envvar_len(env_var);
	name = ft_strndup(env_var, len);
	return (name);
}

// GET BEFORE STR BEFORE ENVVAR
//  * (1) Calculate the length of the string before the environment variable.
//  * (2) If there is nothing before the environment variable,
//		return an empty string.
//  * (3) Otherwise, return the substring before the environment variable.
//  * @param
//  * char *str = "Hello $USER world!";
//  * char *found = str + 6; // This points to '$' in "$USER"
//  * @note
//  * len = found - str; would be 6, because $ is at position 6 in the string
//		"Hello $USER world!" (0-based index).
//  * ft_strndup(str, 6) would return "Hello " (the substring before $USER).
//  * 
//  * @return (char *) the part(string) BEFORE $USER (environment variable).
char	*get_str_before_envvar(char *full_str, char *env_var)
{
	int		front_strlen;
	char	*front_str;

	front_strlen = env_var - full_str;
	if (front_strlen == 0)
		return (ft_strdup(""));
	front_str = ft_strndup(full_str, front_strlen);
	return (front_str);
}

/** GET AFTER ENV_NAME*/
char	*get_str_after_envvar(char *env_var)
{
	int		len;
	char	*new;

	len = ft_envvar_len(env_var);
	new = ft_strdup(env_var + len);
	return (new);
}

// GET ENVVAR_VALUE
//  * Retrieves the value of the environment variable USER.
//  * 
//  * @param 
//  * char	*path : the path name of an environment variable,
//  * Ex: "$USER", ctx (with "USER" = "john"); "john" is the value
//  * 
//  * @note
//  * $? : expands to the exit status (return code) 
//		of the last command executed.
//  * $$ : expands to the process ID (PID) of the shell
//		that is executing the script or command.
char	*get_envvar_value(char *env_var, t_shell *content)
{
	char	*path;
	t_env	*env_variable;
	char	*result;

	path = get_envvar_name(env_var);
	if (path && ft_strcmp(path, "?") == 0)
	{
		result = handle_qmark_exit_status(content);
		free(path);
		return (result);
	}
	else if (path && ft_strcmp(path, "$") == 0)
		return (free(path), handle_dollar_pid());
	env_variable = get_env(path, content->env);
	if (path)
		free(path);
	if (!env_variable || !env_variable->value)
		return (NULL);
	return (ft_strdup(env_variable->value));
}

// HANDLE QMARK_EXIT STATUS ($?)
// 	- If a signal code is present, it takes priority.
//	- If NO signal code is present, the normal exit code is used.
// * @note
// * After return the signal code, it’s reset to 0 to avoid using stale
// 		signal values for future processes.
// * @return string (ft_itoa(code)); If a process was terminated by a signal
// * @return normal exit code (ft_itoa(ctx->exit_code)); If the process
// 		wasn’t terminated by a signal (g_signals.signal_code == 0)
char	*handle_qmark_exit_status(t_shell *content)
{
	int	status_code;

	status_code = 0;
	if (g_signal.signal_code != 0)
	{
		status_code = g_signal.signal_code;
		g_signal.signal_code = 0;
		return (ft_itoa(status_code));
	}
	return (ft_itoa(content->exit_code));
}
//TODO
/** HANDLE DOLLAR SIGN PID ($$)*/
char	*handle_dollar_pid(void)
{
	// return (ft_strdup("program_pid"));
	int	pid;
	pid = getpid();
	return (ft_itoa(pid));
}

/** prs_strjoin*/
char	*prs_strjoin(char *s1, char *s2)
{
	char	*result;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	else if (s2 == NULL)
		return (s1);
	else if (s1 == NULL)
		return (ft_strdup(s2));
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}
