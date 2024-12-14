
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

// # include "../libft/includes/libft.h"
// # include "../libft/includes/ft_printf.h"
// # include "../libft/includes/get_next_line.h"
// # include "../includes/parse_build.h"
// # include "../includes/exec.h"
// # include "../includes/builtins.h"



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
#define PROMPT_NAME "minishell"

# define PROMPT "\001\033[1;36m\002 supershell>$ \001\033[0m\002" //color setting
# define DEFAULT_ENV "SHELL=supershell"

typedef struct s_shell	t_shell;
typedef struct s_env	t_env;
typedef enum e_token_type	t_token_type;
typedef struct s_token	t_token;
typedef struct s_arg	t_arg;
typedef struct s_filename	t_filename;
typedef struct s_exec	t_exec;

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
	char				*cmd;
	t_arg				*args;
	t_filename			*redirs;
	struct s_exec		*next;
	int					fd_in;
	int					fd_out;
}						t_exec;

typedef struct s_signal
{
	int	end_heredoc;
	int	signal_code;

}	t_signal;

extern t_signal g_signal;

# define STDERR 2

////////////////////////////////////////////////////////////

///// ENV /////
/* TEST INPUTS:
USER
PATH
INVALID_VAR
USER PATH
*/
int	ft_env(t_shell *content, t_arg *args);
void	error_env(char *input_line);
void	ft_putstr_fd(char *s, int fd);

int	ft_env(t_shell *content, t_arg *args) //t_shell *content, t_arg *args
{
	t_env	*env;
	char	*input_line;
	
	input_line = args->value;
	if (args)
	{
		error_env(input_line);
		// content->exit_code = CMD_NOT_FOUND;//comment not found=127
		return (CMD_NOT_FOUND);
	}
	while (env)
	{
		if (env->value)
			printf("%s\n",env->env_line);
		env = env->next;
	}
	return (SUCCESS);
}

void	error_env(char *input_line)
{
	ft_putstr_fd("env: ", STDERR);
	ft_putstr_fd(input_line, STDERR);
	ft_putstr_fd(": No such file or directory\n", STDERR);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}