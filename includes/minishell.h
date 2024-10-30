#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include "../libft/includes/ft_printf.h"
# include "../libft/includes/get_next_line.h"

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h> 
# include <readline/readline.h>
# include <readline/history.h>


# define PROMPT "\001\033[1;36m\002supershell>$ \001\033[0m\002" //color setting
# define DEFAULT_ENV "SHELL=supershell"


// typedef struct s_exec
// {

// }	t_exec


typedef struct s_shell
{
	int default_in;
	int default_out;
	unsigned int exit_code;
	pid_t *pids; //Pointer to an array of process IDs of child processes.
	t_exec *exec; //Points to a structure holding command execution details.
	int pid_count;
	int exec_count;
	t_shell env;
	t_shell next;
}	t_shell;


int read_n_loop(t_shell *content);



#endif