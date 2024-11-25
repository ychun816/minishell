/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:25 by yilin             #+#    #+#             */
/*   Updated: 2024/11/25 19:16:37 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include "../libft/includes/ft_printf.h"
# include "../libft/includes/get_next_line.h"

# include "../includes/lexing.h"

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h> 
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

# define SUCCESS 0
# define FAILURE 1
# define FAILURE_VOID 2

// # define BUILD_FAILURE -1

# define PROMPT "\001\033[1;36m\002supershell>$ \001\033[0m\002" //color setting
# define DEFAULT_ENV "SHELL=supershell"

# define SIGNAL_OFFSET 0

typedef struct s_shell	t_shell;
typedef struct s_exec	t_exec;
typedef struct s_filename	t_filename;
typedef struct s_arg	t_arg;
typedef struct s_env	t_env;

typedef struct s_arg
{
	char	*value;
	t_arg	*next;
}	t_arg;

typedef struct s_filename
{
	char	*path;
	t_token_type	type;
	t_filename	*next;
}	t_filename;

typedef struct s_exec
{
	char	*cmd;
	t_arg	*args;
	t_filename	*redirs;
	t_exec	*next;
	int	fd_in;
	int	fd_out;
}	t_exec;

typedef struct s_env
{
	char	*id;
	char	*value;
	char	*env_line;
	t_env	*next;
}	t_env;

typedef struct s_shell
{
	int default_in;
	int default_out;
	unsigned int exit_code;
	pid_t *pids; //Pointer to an array of process IDs of child processes.
	t_exec *exec; //Points to a structure holding command execution details.
	int pid_count;
	int exec_count;
	t_env *env;
}	t_shell;

typedef struct s_signal
{
	int	end_heredoc;
	int	signal_code;
	
}	t_signal;

extern t_signal g_signal;


/*init shell*/
t_shell *init_shell(char *env[]);//initialize & dup env to supershell
int read_n_loop(t_shell *content);
t_env *dup_env(char *env[]);

/*handle env*/
t_env *create_env(char *env_id, char *env_value, char *env_line);
t_env	*set_default_env(void);
char *get_env_id(char *env_line); //If raw points to the beginning of the string (index 0)
char *get_env_value(char *env_line);
int env_add_back(t_env **head, t_env *new);
void	env_free(t_env *env);
t_env	*get_env(char *pathname, t_env *env);

/*lexing*/
t_token	*lex_tokenize_each_wd(char *str, t_shell *content);

/*lexing helper*/
t_token	*create_token(char *token_value, int n, t_token_type type, t_shell *content);
int	ft_1token_len(char *str, t_token_type type);
int ft_token_str_len(char *str);
int ft_quotes_len(char *str, char sd_quote);
t_token_type get_token_type(char *str);
int check_meta_char(char c);

/*token*/
void	token_free(t_token *token);
int	token_add_back(t_token **head, t_token *new_token);

/*parsing*/
int	prs_check_quotes_valid(t_token *token);
int	prs_handle_quotes_n_expand_env(t_token *token);
int	prs_remove_node_null(t_token **head);
int	prs_check_allnodes_null(t_token *token);
int	prs_handle_redir(t_token *token);
int	prs_handle_cmd(t_token *token);
int	prs_handle_heredoc(t_token *token);
void	prs_unlink_error(t_token *token);

t_token	*prs_quotes_to_tokens(char *input_str, t_shell *content);
t_token	*prs_get_quoted_str(char *input_str, char c, t_shell *content);
int	ft_rogue_len(char	*str);

int	prs_expand_env(t_token *token);
char	*prs_tokens_combine(t_token *token);

/*expansion*/
int	count_dollar_sign(char *input_str);
char	*prs_exapnd_env_to_str(char *str, char *envvar_found, t_shell *content);
int	prs_expand_envvar_to_token(t_token *token);

/*expansion helper*/
int	ft_envvar_len(char	*env_var);
char	*get_envvar_pathname(char *env_var);
char	*get_str_before_envvar(char *full_str, char *env_var);
char	*get_str_after_envvar(char *full_str, char *env_var);
char	*get_envvar_value(char *env_var, t_shell *content);

char	*handle_qmark_exit_status(t_shell *content);
char *handle_dollar_pid(void);

char	*prs_strjoin(char *s1, char *s2);

/*heredoc*/
int	prs_init_heredoc(int fd, char *eof_delimiter);


/*build to exec*/
t_exec	*init_build(void);
t_exec *build_for_exec(t_token *token);

/*build helper*/
int	build_listsize(t_exec *exec);
void	build_free_all(t_exec *exec);

/*build - args*/
int	bd_handle_args(t_exec *exec, t_token *token);
t_arg	*arg_create(char *arg_value);
int	arg_add_back(t_arg **head, t_arg *new);
void	arg_free(t_arg *args);

/*buld - filenames*/
int	bd_handle_redirs(t_exec *exec, t_token *token);
t_filename *filename_create(char *pathname, t_token_type type);
int	filename_add_back(t_filename **head, t_filename *new);
void	filename_free(t_filename *filename);

/*signal*/
void	sig_heredoc(int status);


/*free /cleanup */
void	free_all_shell(t_shell *content);


/*test*/
void display_env(t_env *env);



#endif