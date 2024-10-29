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
# define DEF_ENV "SHELL=supershell"

typedef struct s_shell
{
	unsigned char exit_code;


}	t_shell;


int read_n_loop(t_shell *content);



#endif