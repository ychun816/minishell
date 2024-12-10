/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:25 by yilin             #+#    #+#             */
/*   Updated: 2024/12/10 18:54:15 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// printf("🎅TEST current token value: %s\n", current); ///TESTER

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h> 
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <errno.h>

# include "../libft/includes/libft.h"
# include "../libft/includes/ft_printf.h"
# include "../libft/includes/get_next_line.h"

# include "../includes/parse_build.h"
# include "../includes/exec.h"
# include "../includes/builtins.h"



# define SUCCESS 0
# define FAILURE 1
# define FAILURE_VOID 2
// # define BUILD_FAILURE -1
//note: return 0 : success
//note: return 1 : general error
//note: return 2 : Incorrect Usage or Invalid Option

# define IS_DIRECTORY 126
# define CMD_NOT_FOUND 127
# define SIGNAL_OFFSET 128

# define PROMPT "\001\033[1;36m\002 supershell>$ \001\033[0m\002" //color setting
# define DEFAULT_ENV "SHELL=supershell"
# define SIGNAL_OFFSET 0

typedef struct s_shell
{
	int default_in;
	int default_out;
	unsigned char exit_code;
	pid_t *pids; //Pointer to an array of process IDs of child processes.
	t_exec *exec; //Points to a structure holding command execution details.
	int pid_count;
	int exec_count;
	t_env *env;
}	t_shell;

typedef struct s_env
{
	char	*id;
	char	*value;
	char	*env_line;
	t_env	*next;
}	t_env;

// typedef enum e_token_type
// {
// 	INFILE, //0
// 	OUTFILE, //1
// 	HEREDOC, //2 <<
// 	NON_HEREDOC, //3
// 	APPEND,//4
// 	PIPE,//5
// 	STR,//6
// 	SGL_QUOTE,//7
// 	DBL_QUOTE,//8
// 	COMMAND,//9
// 	FILENAME,//10
// 	ARG,//11
// } t_token_type;

// typedef struct s_token
// {
// 	char	*value;
// 	t_shell	*content;
// 	t_token_type	type;
// 	t_token	*next;
// }	t_token;

typedef struct s_signal
{
	int	end_heredoc;
	int	signal_code;
	
}	t_signal;

extern t_signal g_signal;

/*******************************/
/************ MAIN *************/
/*********************************/
/*main (shell)*/ //CHECKED, TO DO: signal
t_shell *init_shell(char *env[]);//initialize & dup env to supershell
int	process_input(t_shell *content, char *line);
int	init_exec(t_shell *content, t_token **token);
int read_n_loop(t_shell *content);
t_env *dup_env(char *env[]);
int check_line_empty(char *line);

/*env*/ //CHECKED
t_env *env_create(char *env_id, char *env_value, char *env_line);
t_env	*set_default_env(void);
char *get_env_id(char *env_line); //If raw points to the beginning of the string (index 0)
char *get_env_value(char *env_line);
int env_add_back(t_env **head, t_env *new);
void	env_free(t_env *env);
void	env_del_one(t_env *env);

/********************************/
/************ LEXING ************/
/*********************************/
/*lexing*/ //CHECKED
t_token	*lex_tokenize_each_wd(char *str, t_shell *content);
t_token	*lexing(t_shell *content, char *input_line);

/*lexing helper*/ //CHECKED
t_token_type lex_get_token_meta_type(char *str);
int	lex_ft_1tokenlen(char *str, t_token_type type);
int ft_token_str_len(char *str);
int ft_quotes_len(char *str, char sd_quote);
int check_meta_char(char c);

/*token*/ //CHECKED
t_token	*token_create(char *token_value, int n, t_token_type type, t_shell *content);
int	token_add_back(t_token **head, t_token *new_token);
void	token_free(t_token *token);

/*********************************/
/************ SIGNALS ************/
/*********************************/
void	sig_int_handler(int status);
void	sig_init_signals(void);
void	sig_heredoc(int status);
void	sig_exec(int status);
int		sig_event(void);


/*******************************/
/************ FREE ************/
/******************************/
/*free /cleanup */
void	free_all_shell(t_shell *content);
void	free_after_process(t_shell *content, t_token *token);

/******************************/
/*********** TESTERS **********/
/******************************/
/*parsing*/
int test_read_n_loop(t_shell *content);
void	test_display_env(t_env *env);
void test_token_append(t_token **head, char *value, int n, t_token_type type, t_shell *content);
void	test_print_tokens(t_token *head);
t_token *test_tokenize_input(char *input, t_shell *shell);
const char *test_tokentype_to_str(t_token_type type);
/*exec*/
void test_print_exec(t_exec *exec);


#endif