/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:21 by yilin             #+#    #+#             */
/*   Updated: 2024/12/05 14:09:51 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H

# include "../includes/minishell.h"

typedef struct s_shell	t_shell;
typedef struct s_token	t_token;

typedef enum e_token_type
{
	INFILE, //0
	OUTFILE, //1
	HEREDOC, //2 <<
	NON_HEREDOC, //3
	APPEND,//4
	PIPE,//5
	STR,//6
	SGL_QUOTE,//7
	DBL_QUOTE,//8
	COMMAND,//9
	FILENAME,//10
	ARG,//11
} t_token_type;

typedef struct s_token
{
	char	*value;
	t_shell	*content;
	t_token_type	type;
	t_token	*next;
}	t_token;

#endif
