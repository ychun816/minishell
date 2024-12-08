/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:15 by yilin             #+#    #+#             */
/*   Updated: 2024/12/08 19:07:00 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_signal g_signal = {0};


int check_line_empty(char *line)
{
	if (line[0] == '\0')
		return (1);
	return (0);
}

/** DUP ENV 
 * inside dup_edit_env:
 * TODO : if env exists => edit the value, otherwise create it
*/
t_env *dup_env(char *env[])
{
	int	i; // Index for iterating through the environment variables
	char *env_id; // Variable to hold the identifier of the environment variable(the part before the =)
	char *env_value; // Variable to hold the value of the environment variable(environment variable’s contents, after =)
	t_env *res_env; 
	t_env *tmp;

	i = 0;
	res_env = set_default_env();
	//Check if the environment pointer is NULL or points to an empty string
	// if (!env || (env && *env == NULL)) //env points to a valid location; But the environment variable list is empty.
	// 	return (set_default_env());
	while (env[i])// Loop through each environment variable
	{
		env_id = get_env_id(env[i]);//get identifier
		env_value = get_env_value(env[i]);//get value
		tmp = env_create(env_id, env_value, ft_strdup(env[i])); // Create a new environment variable structure (tmp)
		//if fail -> return NULL
		if (!tmp)
			return (NULL);
		env_add_back(&res_env, tmp);
		i++;
	}
	return (res_env);
}

/* INIT SHELL*/
t_shell *init_shell(char *env[])//initialize & dup env to supershell
{
	t_shell *content; // Declare a pointer to a context structure
	
	content = malloc(sizeof(t_shell));// Allocate memory for the context structure
	if (!content)
		return (NULL);
	content->env = dup_env(env); // Duplicate the environment variables into the context
	if (!(content->env))
	{
		free(content);
		return (NULL);
	}
	content->default_in = STDIN_FILENO; // Set default input to standard input
	content->default_out = STDOUT_FILENO; // Set default output to standard output
	content->pids = NULL;
	content->exec = NULL;
	content->pid_count = 0;
	content->exec_count = 0;
	return (content);
}

/** PROCESS INPUT
 * Input → Lexing → Parsing → Execution Setup → Execution → Cleanup.
 * STEP 1: LEXING:
 * Convert the input string 'line' into tokens (words, operators, etc.)
 * STEP 2: PARSING: 
 * Check if the tokenized input has correct syntax and prepare it for execution
 * STEP 3: EXECUTION SETUP PHASE
 * Prepare the tokens for execution
 * STEP 4: EXECUTION
 * STEP 5: CLEANUP
 */
int	process_input(t_shell *content, char *line)
{
	t_token	*token;
	
	// 1. LEXING
	token = lexing(content, line);
	free(line);
	if (!token)
		return (SUCCESS); 
	// 2. PARSING
	if (parsing(&token) != 0)
	{
		token_free(token);
		if (parsing(&token) == FAILURE_VOID)//void or special case
			return (SUCCESS);//0
		return (FAILURE);//1
	}
	// 3. BULD2EXEC
	if (init_exec(content, &token) != 0)////TO CHECKKKKKKKKKKKKKKK!!!!
		return (FAILURE);//1
	// 4. EXEC
	exec(content);
	// 5. CLEAN
	free_after_process(content, token);
	return (SUCCESS);//0
}

/** INIT EXEC */ ///CHECK LATER
int	init_exec(t_shell *content, t_token **token)
{
	content->exec = build_to_exec(*token);
	token_free(*token);
	*token = NULL; //Set the original token pointer to NULL to avoid dangling pointers
	if (!content->exec)
		return (FAILURE);//1
	content->exec_count = ft_build_lstsize(content->exec); //Set the count of executions based on the size of the exec list
	//Allocate memory for storing process IDs, one for each command to be executed, plus one extra slot
	content->pids = malloc(sizeof(pid_t) * (content->exec_count + 1));
	if (!content->pids)
		return (FAILURE);//1
	content->pid_count = 0; //Initialize the count of processes launched to 0
	return (SUCCESS);//0
}


/** FREE AFTER PROCESS 
 * (1) free token
 * (2) free what's inside shell: free build, pid
 * @note
 * The pids array in the context (ctx) likely stores the process IDs of child processes that the shell has spawned
 * Setting to NULL
 * -> Preventing Dangling Pointers
 * -> Safety in Future Operations
 * -> Improving Debugging
 * -> Avoiding Double Free Errors
 * -> Preventing Unintended Access After Freeing
 * 
*/
void	free_after_process(t_shell *content, t_token *token)
{
	if (token)
		token_free(token);
	if (content)
	{
		//exec
		if (content->exec)
		{
			build_free_all(content->exec);
			content->exec = NULL;
		}
		//pid
		if (content->pids)
		{
			free(content->pids);
			content->pids = NULL;
		}
	}
}


/** === OG READ N LOOP === */
int read_n_loop(t_shell *content)
{
	char *line;

	(void)content;
	line = NULL;
	while (1)
	{
		sig_init_signals(); // Set up signal handling for each loop iteration
		line = readline(PROMPT);
		if (!line)
			break ;// Exit the loop to end the shell
		if (ft_strncmp(line, "exit", 4) == 0) //////////TESTER//////////
			break ;
		// if (ft_strncmp(line, "env", 3) == 0) //////////TESTER//////////
		// 	test_display_env(content->env);
		else if (check_line_empty(line) == 0)// check if line valid
		{
			add_history(line);//add history //REMEMBER TO CLEAR 
			if (process_input(content, line) != 0)//process pipeline ///////EXEC PART
			{
				ft_putstr_fd("Parcing Error\n", 2);
				content->exit_code = 2;
			}
			line = NULL;//reset line
		}
		if (line)
			free(line);
	}
	return (0); //return if loop end ((i.e., on EOF or NULL input))
}

/* FREE ALL SHELL*/
void	free_all_shell(t_shell *content)
{
	// Check if ctx (shell context) exists before freeing its contents
	if (content)
	{
		if (content->exec)//exec
			build_free_all(content->exec);
		if (content->env)//env
			env_free(content->env);
		if (content->pids)//pids
			free(content->pids);
		free (content);//content
	}
}

int main(int ac, char *av[], char *env[])
{
	t_shell *content;

	content = NULL;
	(void)ac;
	(void)av;
	content = init_shell(env);//initialize everthing to 0/NULL
	if (!content)
		return (EXIT_FAILURE);
	read_n_loop(content);//strat loop -> readline(PROMPT)
	free_all_shell(content);
	ft_putstr_fd("exit\n", 2);//or 1
	return (EXIT_SUCCESS);
}
