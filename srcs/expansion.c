/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:44:48 by yilin             #+#    #+#             */
/*   Updated: 2024/11/13 15:14:41 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** EXAMPLE INPUT:
 * input string = "Hello $USER, welcome to the shell!";
 * "$USER" : Environment Variable (env_name)
 * "Hello" : String BEFORE (env_name)
 * ", welcome to the shell!" : String AFTER (env_name)
 */

#include "minishell.h"

/** PARSE- FT_COUNT_DOLLAR_SIGN
 * Counts how many times the dollar sign ($) appears in a string.
 * This is used to determine how many environment variables might need to be expanded in the string.
 * 
 */



/**  PARSE- EXPAND_ENV_TO_STR (ps_convert_to_env)
 * - Expands an environment variable in a string.
 * - It separates the string into parts before and after the environment variable, 
 * retrieves the variable's value from the environment, 
 * and then joins these parts back together with the expanded environment variable.
 */


/** PARSE- HANDLE ENV
 * Handles the process of expanding environment variables in a single token.
 * It looks for $ symbols in the token’s value, 
 * calls ps_convert_to_env to expand any environment variables, and updates the token's value accordingly.
 */



/** PARSE- EXPAND ENV
 * Loops through a list of tokens and expands environment variables in tokens of type STRING or DOUBLEQUOTE.
 * It calls (ps_handle_env) to handle the expansion for each token that requires it.
 * 
 */
