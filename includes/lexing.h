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
	NON_TERM_HEREDOC,
	APPEND,
	PIPE,
	STRING,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
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