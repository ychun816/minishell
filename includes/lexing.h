/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:21 by yilin             #+#    #+#             */
/*   Updated: 2024/11/20 16:12:22 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H

# include "../includes/minishell.h"

typedef struct s_shell	t_shell;
typedef struct s_token	t_token;

typedef enum e_token_type
{
	INFILE,
	OUTFILE,
	HEREDOC,
	NON_HEREDOC,
	APPEND,
	PIPE,
	STR,
	SGL_QUOTE,
	DBL_QUOTE,
	COMMAND,
	FILENAME,
	ARG,
} t_token_type;

typedef struct s_token
{
	char	*value;
	t_shell	*content;
	t_token_type	type;
	t_token	*next;
}	t_token;

#endif