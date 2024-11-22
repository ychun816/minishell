/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:44:48 by yilin             #+#    #+#             */
/*   Updated: 2024/11/22 17:56:35 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* EXAMPLE INPUT:
 * input string = "Hello $USER, welcome to the shell!";
 * "$USER" : Environment Variable (env_name)
 * "Hello" : String BEFORE (env_name)
 * ", welcome to the shell!" : String AFTER (env_name)
 */

#include "minishell.h"

/** PARSE- FT_COUNT_DOLLAR_SIGN
 * Counts how many times the dollar sign ($) appears in a string.
 * This is used to determine how many environment variables might need to be expanded in the string.
 */
int	count_dollar_sign(char *input_str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input_str[i])
	{
		if (input_str[i] == '$')
			count++;
		i++;
	}
	return (count);
}

/**  PARSE- EXPAND_ENV_TO_STR (ps_convert_to_env) TODO TODO TODO
 * Replaces a single $VAR in a string with its corresponding environment variable value.
 * 
 * It separates the string into parts before and after the environment variable, 
 * retrieves the variable's value from the environment, 
 * and then joins these parts back together with the expanded environment variable.
 * 
 * @param 
 * char *before_env;  // The part of the string before `$VAR`
 * char *env_var;     // The value of the variable from the environment
 * char *after_env;   // The part of the string after `$VAR`
 * char *new;         // The newly constructed string
 * 
 * (1) get values for before_env / env_var / after_env ; return NULL if fail
 * (2) strjoin/append [befor_env & env_var] & [after_env] ; free after use or invalid
 * (3) 
 * 
 * @note REMEMBER TO FREE USED / INVALID POINTERS!
 * 
 * @return String with $VAR replaced with its value.
 */
char	*prs_exapnd_env_to_str(char *str, char *envvar_found, t_shell *content)
{
	char	*before_envvar;
	char	*envvar_value;
	char	*after_envvar;
	char	*new;

	before_envvar = get_str_before_envvar(str, envvar_found);
	if (!before_envvar)
		return NULL;
	envvar_value = get_envvar_value((envvar_found + 1), content); //ps_get_env_var(found + 1, ctx);
	after_envvar = get_str_after_envvar(str, envvar_value);
	if (!after_envvar)
		return (free(before_envvar), free(envvar_value), NULL);
	new = prs_strjoin(before_envvar, envvar_value);
	if (!new)
		return (free(after_envvar), NULL);
	new = prs_strjoin(new, after_envvar);
	if (!envvar_value)
		free(envvar_value);
	return (free(after_envvar), new);
}

//// envvar_value = get_envvar_value((envvar_found + 1), content); 


/** PARSE- HANDLE ENV
 * Expands all occurrences of $VAR in a single token's value.
 * 
 * Handles the process of expanding environment variables in a single token.
 * It looks for $ symbols in the token’s value, 
 * calls ps_convert_to_env to expand any environment variables, and updates the token's value accordingly.
 * 
 * (1) Loop thru value (by decrement n_dollar)
 * -1 Stop if NO `$` || NO expansions left
 * -2 Stop if Edge cases ie. standalone `$` (environment variable named $ doesn't exist.)
 * -3 Store the current value of the token for later freeing
 * -4 Convert the envvar_found environment variable into its value
 * -5 If the expansion results in an empty string, clean up and set `new` to NULL.
 * -6 Update the token's value with the new string.
 * -7 Free used current pointer
 * 
 * @param
 * char *tmp;
 * char *envvar_found;  // Pointer to the `$` symbol in the string
 * char *new;
 * int n_dollar;       // Number of `$` symbols in the token's value
 * 
 * @return Token value updated with $VAR expansions.
 */
int	prs_expand_envvar_to_token(t_token *token)
{
	char	*current;
	char	*envvar_found;
	char	*new;
	int	n_dollar;

	n_dollar = count_dollar_sign(token->value); //get the value inside token(STR)
	while(token->value != NULL)
	{
		envvar_found = ft_strchr(token->value, '$');
		if (envvar_found == NULL || n_dollar == 0)
			break ;
		if (ft_strcmp(envvar_found, "$") == 0)
			break ;
		current = token->value;
		envvar_found = prs_exapnd_env_to_str(token->value, envvar_found, token->content);
		if (new == NULL || new[0] == '\0')
		{
			free(new);
			new = NULL;
		}
		token->value = new;
		free(current);
		n_dollar--;
	}
	return (SUCCESS);	
}



