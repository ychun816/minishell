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

# define PROMPT "\001\033[1;36m\002supershell>$ \001\033[0m\002" //color setting
# define DEFAULT_ENV "SHELL=supershell"

typedef struct s_shell	t_shell;
typedef struct s_exec	t_exec;
typedef struct s_filenames	t_filenames;
typedef struct s_args	t_args;
typedef struct s_env	t_env;

typedef struct s_args
{
	char	*value;
	t_args	*next;
}	t_args;

typedef struct s_filenames
{
	char	*path;
	t_token_type	type;
	t_filenames	*next;
}	t_filenames;

typedef struct s_exec
{
	char	*cmd;
	t_args	*args;
	t_filenames	*redirs;
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
	t_env env;
}	t_shell;


t_env *create_env(char *env_id, char *env_value, char *env_line);
t_env set_default_env(void);
char *get_env_id(char *env_line); //If raw points to the beginning of the string (index 0)
char *get_env_value(char *env_line);
int env_add_back(t_shell *head, t_shell *new);


t_shell int_shell(char *env[]);//initialize & dup env to supershell



t_env dup_env(char *env[]);
t_shell int_shell(char *env[]);//initialize & dup env to supershell
int read_n_loop(t_shell *content);



#endif