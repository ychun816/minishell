#include "minishell.h"

t_shell dup_env(char *env[])
{
	int	i; // Index for iterating through the environment variables
	char *identifier; // Variable to hold the identifier of the environment variable
	char *value; // Variable to hold the value of the environment variable
	t_shell *dup_env; 
	t_shell tmp;

	i = 0;
	dup_env = NULL;
	


	return (dup_env);
}



t_shell int_shell(char *env[])//initialize & dup env to supershell
{
	t_shell content;// Declare a pointer to a context structure
	// Allocate memory for the context structure
	// if fail -> return NULL 

	//duplicate env

	//initialize everything


}

int read_n_loop(t_shell *content)
{
	char *line;

	(void)content;
	line = NULL;
	while (1)
	{
		// sig_init_signals(); // Set up signal handling for each loop iteration
		line = readline(PROMPT);
		if (!line)
			break ;// Exit the loop to end the shell
		else if (check_line(line) == 0)// check if line valid
		{
			add_history(line);//add history
			if (process_pipeline(content, line) != 0)//process pipeline
			{
				ft_putstr_fd("Parcing Error\n", 2);
				content->exit_code = 2;
			}
			line = NULL;//reset line
		}
		if (line)//if line not null -> free
			free(line);
	}
	return (0); //return if loop end ((i.e., on EOF or NULL input))
}

int main(int ac, char *av[], char *env[])
{
	t_shell *content = NULL;

	(void)ac;
	(void)av;
	(void)env;
	content = init_content(env);//initialize everthing to 0/NULL
	if (!content)
		return (EXIT_FAILURE);
	read_n_loop(content);//strat loop -> readline(PROMPT)
	// free_all(env);
	ft_putstr_fd("exit\n", 2);//or 1
	return (EXIT_SUCCESS);
}
