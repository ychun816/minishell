/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:14:21 by yilin             #+#    #+#             */
/*   Updated: 2024/11/13 18:01:33 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** EXAMPLE INPUT:
 * input string = "Hello $USER, welcome to the shell!";
 * "$USER" : Environment Variable (env_name)
 * "Hello" : String BEFORE (env_name)
 * ", welcome to the shell!" : String AFTER (env_name)
 */

#include "minishell.h"

/** FT_DOLLAR_ENVNAME_LEN
 * Counts the length of "USER" after $.
 * Starts from the given string `found`
 * Checks for valid characters that can make up an environment variable's name.
 * 
 * (1) If the first character is a digit, '?' or '$', it's invalid as an environment variable name.
 * (2) Iterate through the string, checking if each character is valid for an environment variable name.
 * 		// A valid name can contain letters, digits, and underscores.
		// If a character is neither alphanumeric nor an underscore, stop counting.
 * 	
 * @return (int) length after $ sign
 */
int	ft_envname_len(char	*after_dollar)
{
	int	len;
	len = 0;

	if (ft_isdigit(after_dollar[len]) == 1 || after_dollar[len] == '?' || after_dollar[len] == '$')
		return (FAILURE);
	while (after_dollar[len])
	{
		if (ft_isalnum(after_dollar[len]) == 1 && after_dollar[len] != '_')
			break ;
		len++;
	}
	return (len);
}

/** GET DOLLAR_ENVNAME
 * Extracts the variable name "USER" from "$USER".
 * Ex: Get the variable name "USER" from "$USER".
 * 
 * @return (char *)
 */
char	*get_envname(char *after_dollar)
{
	int	len;
	char	*env_name;

	len = ft_envname_len(after_dollar);
	env_name = ft_strndup(after_dollar, len);
	return (env_name);
}

/** GET BEFORE ENVNAME
 * (1) Calculate the length of the string before the environment variable.
 * (2) If there is nothing before the environment variable, return an empty string.
 * (3) Otherwise, return the substring before the environment variable.
 * 
 * @note
 * char *str = "Hello $USER world!";
 * char *found = str + 6; // This points to '$' in "$USER"
 * len = found - str; would be 6, because $ is at position 6 in the string "Hello $USER world!" (0-based index).
 * ft_strndup(str, 6) would return "Hello " (the substring before $USER).
 * 
 * Returns the part(string) BEFORE $USER (environment variable).
 * @return (char *)
 */
char	*get_str_before_envname(char *full_str, char *after_dollar)
{
	int	front_strlen;
	char	*front_str;

	front_strlen = after_dollar - full_str;
	front_str = ft_strndup(full_str, front_strlen);
	if (front_strlen == 0)
		return (ft_strdup(""));
	return (front_str);
}

/** GET AFTER ENV_NAME
 * Returns the part(string) AFTER $USER (environment variable).
 *  
 */
char	*get_str_after_envname(char *full_str, char *after_dollar)
{
	int	envname_len;
	char	*after_str;
	
	envname_len = ft_envname_len(after_dollar);
	// Skip the environment variable name itself.
	//By adding envname_len (4) to after_dollar, the pointer now points to the character after USER
	after_str = ft_strdup(full_str + envname_len); 
	return (after_str);
}

/** GET ENVNAME_VALUE
 * Retrieves the value of the environment variable USER.
 * 
 * @param char	*envname_path : the name of an environment variable,
 * Ex: "$USER", ctx (with "USER" = "john"); "john" is the value
 * 
 * @note
 * $? : 
 * $$ : 
 * 
 */
char	*get_envname_path(char *after_dollar, t_shell content)
{
	char	*envname_path;
	t_env	*env_variable;
	int	code;
	
	// Get the name of the environment variable from the `after_dollar$` string.
	envname_path = get_envname(after_dollar);

	// Check if the variable is "?" (special variable for the last command's exit code).
	if (envname_path && ft_strcmp(envname_path, "?") == 0)
	{
		free(envname_path); // Free `path` as it's no longer needed.
		if (g_signal.signal_code != 0) // If a signal exit code is present, use it, then reset the signal code.
			return (handle_qmark(content));
	}
	// Check if the variable is "$" 
	else if (envname_path && ft_strcmp(envname_path, "$") == 0)
	{
		free(envname_path);
		return (handle_dollar_sign());
	}


	return (envname_path);
}

/** envname_handle_question_mark
 * 
 * @note
 * If a signal terminated the process, 
 * g_signals.signal_code will hold that signal's exit status (e.g., 130 for SIGINT). 
 * The function returns that signal code.
 * 
 * - If a signal code is present, it takes priority.
 * - If NO signal code is present, the normal exit code is used.
 * 
 * - If a process was terminated by a signal, the function returns the signal code as a string (ft_itoa(code)).
 * - If the process wasn’t terminated by a signal (g_signals.signal_code == 0), it returns the normal exit code (ft_itoa(ctx->exit_code)).
 * - After returning the signal code, it’s reset to 0 to avoid using stale signal values for future processes.
 * 
 * 
*/
char	*handle_question_mark(t_shell *content)
{
	int status_code;

	if (g_signal.signal_code != 0)
	{
		status_code = g_signal.signal_code;
		//resets the signal code after it has been processed.
		//resetting it ensures that it does not incorrectly persist or interfere with future commands or processes.
		g_signal.signal_code = 0;
		return (ft_itoa(status_code));
	}
	return (ft_itoa(content->exit_code));
}

/** handle dollar sign
 * 
 */
char *handle_dollar_sign(t_shell content)
{

	return ();	
}