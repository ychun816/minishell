#include <stdio.h>

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


void	print_exec_list(t_exec *exec)
{
	int	i;
	t_filename *redirs = exec->redirs;

	while (exec != NULL)
	{
		// ? -> "else if"
		//condition ? expression_if_true : expression_if_false;
		printf("COMMMAND: %s\n", exec->cmd ? exec->cmd : "(null)");

		// Print arguments if they exist
		if (exec->args)
		{
			printf("ARGS: ");
			// Loop through the arguments array and print each one
			for (i = 0; exec->args[i] != NULL; i++)
				printf("%s ", exec->args[i]);
			printf("\n");
		}
		else
			printf("Arguments: (null)\n"); // Indicate no arguments if the args array is NULL
		if (exec->redirs) // Print redirections if they exist
		{
			printf("Redirections:\n");
			while (redirs)
			{
				printf("\tType: %d, File: %s\n", redirs->type, redirs->path);
				redirs = redirs->next;
			}
		}
		else
			printf("Redirections: (null)\n");
		exec = exec->next;
		if (exec != NULL) // Check if there is another node to avoid printing an extra PIPE
			printf("=== PIPE ===\n");
	}

}

// Test function for build_for_exec
void test_build_for_exec()
{
	// Create a mock token list to simulate a pipeline of commands and arguments

	// First command: "ls"
	t_token token1 = {COMMAND, "ls", NULL};
	// Argument for "ls": "-l"
	t_token token2 = {ARG, "-l", NULL};
	// Pipe symbol
	t_token token3 = {PIPE, "|", NULL};
	// Second command: "grep"
	t_token token4 = {COMMAND, "grep", NULL};
	// Argument for "grep": "file"
	t_token token5 = {ARG, "file", NULL};
	// Pipe symbol
	t_token token6 = {PIPE, "|", NULL};
	// Third command: "wc"
	t_token token7 = {COMMAND, "wc", NULL};
	// Argument for "wc": "-l"
	t_token token8 = {ARG, "-l", NULL};
	
	// Link the tokens to form a linked list
	token1.next = &token2; // "ls" -> "-l"
	token2.next = &token3; // "-l" -> "|"
	token3.next = &token4; // "|" -> "grep"
	token4.next = &token5; // "grep" -> "file"
	token5.next = &token6; // "file" -> "|"
	token6.next = &token7; // "|" -> "wc"
	token7.next = &token8; // "wc" -> "-l"
	token8.next = NULL;    // "-l" -> NULL (end of the list)
	
	// Call build_for_exec with the token list to construct the t_exec list
	t_exec *exec = build_for_exec(&token1);
	
	// Print the resulting t_exec linked list for verification
	printf("Testing build_for_exec...\n");
	print_exec_list(exec);
}