#include "minishell.h"

// //////////////////////////////////////
// /// FINAL TEST IN LOOP             ///
// //////////////////////////////////////
/* TEST INPUT
[HACKMD] Test Input for Minishell (lexing/parsing/build for exec):
https://hackmd.io/@QBrv51OvRPqs9dJjL2YIig/BJiOL9xEye
*/
int main(int ac, char *av[], char *env[])
{
	t_shell *content;

	content = NULL;
	(void)ac;
	(void)av;
	content = init_shell(env);//initialize everthing to 0/NULL
	if (!content)
		return (EXIT_FAILURE);
	test_read_n_loop(content);//strat loop -> readline(PROMPT)
	free_all_shell(content);
	ft_putstr_fd("exit\n", 2);//or 1
	return (EXIT_SUCCESS);

}

// //////////////////////////////////////
// /// TEST: BUILD to EXEC            /// ///CHECKED!!
// //////////////////////////////////////

// int main(int ac, char *av[], char *env[])
// {
//     (void)ac;
// 	// (void)av;
// 	t_shell *content = init_shell(env);
// 	if (!content) 
// 	{
// 		fprintf(stderr, "Failed to initialize shell\n");
// 		return (1);
// 	}

	///// /** 1. TEST: init build */ /////
	// t_exec *exec = init_build();
	// if (exec)
	// {
	// 	printf("[test_init_build] SUCCESS: t_exec initialized properly\n");
	// 	free(exec);
	// }
	// else
	// 	printf("[test_init_build] FAILURE: t_exec initialization failed\n");

////////////////////////////////////////////////////////////////////////////////

	///// /** 2. TEST: bd_handle_cmd */ /////    /////TODO: TODO: TODO: CHECK LATER!!
	// t_exec exec = {0};
	// t_token token = {av[1], NULL, COMMAND, NULL};

	// int result = bd_handle_cmd(&exec, &token);
	// if (result == SUCCESS) //&& exec.cmd //&& ft_strcmp(exec.cmd, av[1]) == 0
	// 	printf("[test_bd_handle_cmd] SUCCESS: Command '%s' set correctly\n", exec.cmd);
	// else
	// 	printf("[test_bd_handle_cmd] FAILURE: Command was not set correctly\n");

	// free(exec.cmd);

////////////////////////////////////////////////////////////////////////////////

	///// /** 3. TEST: bd_handle_redirs */ /////

    // Initialize exec structure and token
    // t_exec exec = {0};
    // t_token *tokens = NULL;
    // t_token *current_token = NULL;
    // t_shell shell_context = {0};
    // int result = 0;
	/*
	"command < input.txt"
	The tokens would be something like:
    - token: COMMAND (representing command)
    - token.next: INFILE (representing <)
    - token.next.next: "input.txt" (file path)
	*/
	/* TEST INPUT
	VALID
	1. "echo Hello World > output.txt"    => Valid: The program should write "Hello World" to output.txt
	2. "echo Hello World >> append.txt"   => Valid: The program should append "Hello World" to append.txt
	3. "ls -l > output.txt >> append.txt" => Valid: The program should first write the ls -l output to output.txt, then append to append.txt
	INVALID
	//4. "echo Hello World >>"              => Invalid: The program should flag an error as no file is specified after the > operator.
	//5. "ls -l > output.txt > output.txt"  => Invalid: The program should flag an error as the same file (output.txt) is being redirected twice.
	//6. "echo Hello World >>"			  => Invalid: The program should flag an error as no file is specified after the >> operator
	//7. "echo Hello World				  => ls -l > output.txt"`
	//8. "echo Hello World > output.txt >> output.txt" =>  Invalid: The program should flag an error as it tries to overwrite and append to the same file.
	*/

    // Tokenize the input string
    // char *cmd_input = av[1];
    // char *token_str = strtok(cmd_input, " ");
    // while (token_str != NULL) {
    //     t_token *new_token = token_create(token_str, strlen(token_str), STR, &shell_context);
    //     if (!tokens) {
    //         tokens = new_token;
    //         current_token = tokens;
    //     } else {
    //         current_token->next = new_token;
    //         current_token = new_token;
    //     }
    //     token_str = strtok(NULL, " ");
    // }

    // // Handle redirections
    // result = bd_handle_redirs(&exec, tokens);  // Process the tokens for redirections
    
    // if (result == 0) 
	// {
    //     printf("[test_bd_handle_redirs] SUCCESS: Redirection '%s' added correctly (type: %s)\n", tokens->value, test_tokentype_to_str(tokens->type));
    // } else if (result == 1) 
	// {
    //     printf("[test_bd_handle_redirs] FAILURE: Invalid redirection or no redirection processed\n");
    // } else 
	// {
    //     printf("[test_bd_handle_redirs] FAILURE: Redirection not processed\n");
    // }

    // // Free allocated memory for tokens
    // current_token = tokens;
    // while (current_token) {
    //     t_token *temp = current_token;
    //     current_token = current_token->next;
    //     free(temp->value);
    //     free(temp);
    // }

    // // Cleanup and return
    // free(exec.cmd); 

////////////////////////////////////////////////////////////////////////////////

	///// /** 4. TEST: bd_handle_args */ /////
    // t_exec exec = {0};
    // t_token *tokens = NULL;
    // t_token *current_token = NULL;
    // t_shell shell_context = {0};
    // int result = 0;

	// // Tokenize the input string (argument example)
    // char *cmd_input = av[1];
    // char *token_str = strtok(cmd_input, " ");
    // while (token_str != NULL) 
	// {
    //     t_token *new_token = NULL;

    //     // Tokenize as ARG type (you can adjust based on actual argument parsing logic)
    //     new_token = token_create(token_str, strlen(token_str), ARG, &shell_context);

    //     // Link the new token into the list
    //     if (!tokens) 
	// 	{
    //         tokens = new_token;
    //         current_token = tokens;
    //     } else {
    //         current_token->next = new_token;
    //         current_token = new_token;
    //     }
    //     // Move to the next token
    //     token_str = strtok(NULL, " ");
    // }
    // // Handle arguments
    // result = bd_handle_args(&exec, tokens);
    // // Check if arguments were added correctly
    // if (result == 0 && exec.args) 
	// {
    //     // Print out all the arguments stored in exec.args
    //     t_arg *arg = exec.args;
    //     while (arg) 
	// 	{
    //         printf("[test_bd_handle_args] SUCCESS: Argument '%s' added correctly\n", arg->value);
    //         arg = arg->next;
    //     }
    // } 
	// else if (result == -1) 
    //     printf("[test_bd_handle_args] FAILURE: Argument not processed\n");
	// else 
    //     printf("[test_bd_handle_args] FAILURE: No argument found\n");

    // // Free allocated memory for tokens
    // current_token = tokens;
    // while (current_token) 
	// {
    //     t_token *temp = current_token;
    //     current_token = current_token->next;
    //     free(temp->value);
    //     free(temp);
    // }

    // // Cleanup and return
    // if (exec.args) 
	// {
    //     t_arg *arg = exec.args;
    //     while (arg) 
	// 	{
    //         t_arg *temp = arg;
    //         arg = arg->next;
    //         free(temp->value);
    //         free(temp);
    //     }
    // }

////////////////////////////////////////////////////////////////////////////////

	///// /** 5. TEST: build_for_exec */ /////
	/* TEST INPUT
	1. "echo Hello World"
	Command: echo
	Argument: Hello
	Argument: World

	2. "echo Hello > output.txt"
	Command: echo
	Argument: Hello
	Redirection: output.txt (type OUTFILE) (0)
	
	3. "ls -l -a"
	Command: ls
	Argument: -l
	Argument: -a
	
	4. "cat < input.txt"
	Command: cat
	Redirection: input.txt (type OUTFILE) 1

	5. "ls -l | grep txt"
	Command: ls
	Argument: -l
	Command: grep
	Argument: txt

	6. "cat < input.txt > output.txt"
	Command: cat
	Redirection: input.txt (type INFILE) 1
	Redirection: output.txt (type OUTFILE) 0
	
	7. "echo Hello >> output.txt"
	Command: echo
	Argument: Hello
	Redirection: output.txt (type HEREDOC) 2

	8. "echo Hello > output.txt >> append.txt"
	Command: echo
	Argument: Hello
	Redirection: output.txt (type OUTFILE) 0
	Redirection: append.txt (type HEREDOC) 2

	9. "cat file.txt | grep search > output.txt"
	Command: cat
	Argument: file.txt
	Command: grep
	Argument: search
	Redirection: output.txt (type OUTFILE) 0

INVALID
	10. "echo Hello >"
	11. "ls -l |"
	*/
    // t_token *tokens = NULL;
    // t_token *current_token = NULL;
    // t_shell *shell_context = {0};  // Shell context, could be set as needed

    // Tokenize the input string (argument example)
    // char *test_input = av[1];
    // char *token_str = strtok(cmd_input, " ");

	//tokenize input
	// ///// LEXING /////
	// printf("\n===  🍿 TEST LEXING    ===\n");
    // t_token *lexed_tokens = lexing(content, test_input);  // Pass the test input to lexing function
	// test_print_tokens(lexed_tokens);
	// printf("\n========================\n");

	// ///// PARSING /////
	// printf("\n===  🥐 TEST PARSING   ===\n");
    // // Print tokens before parsing
    // printf("= Tokens BEFORE parsing: =\n");
    // test_print_tokens(lexed_tokens);
    // // Call the parsing function
    // int result = parsing(&lexed_tokens);
    // // Print result of parsing
	// printf("\n= Tokens AFTER parsing: =\n");
	// test_print_tokens(lexed_tokens);
    // if (result == 0) //SUCCESS
    //     printf("🢂 🤙 Parsing Successful!!\n");
    // else if (result == FAILURE)
    //     printf("🢂 👎 Parsing Failed (General Error)!\n");
    // else if (result == FAILURE_VOID){
    //     printf("🢂 🫶 Parsing Failed (Failure void error)!\n");
	// }
	// printf("\n========================\n");

    // // Call the function (build_for_exec)
    // t_exec *result_exec = build_for_exec(lexed_tokens);

	// printf("\n=== 🍰 PRINT ALL EXEC  ===\n");
    // // Check if the result is valid and print the result using the test_print_exec function
    // if (result_exec) 
	// {
    //     printf("[test_build_for_exec] SUCCESS: Executable built successfully!!\n");
    //     test_print_exec(result_exec); // Print details of the exec structure
    // } 
	// else
    //     printf("[test_build_for_exec] FAILURE: Failed to build exec structure.\n");
	// printf("\n");

    // // Free allocated memory for tokens
    // current_token = tokens;
    // while (current_token) 
	// {
    //     t_token *temp = current_token;
    //     current_token = current_token->next;
    //     free(temp->value);
    //     free(temp);
    // }

    // // Cleanup and return
    // if (result_exec) 
	// {
    //     // Clean up exec structure (ensure to free all allocated memory in t_exec, t_filename, t_arg)
    //     if (result_exec->cmd) free(result_exec->cmd);
    //     t_filename *redir = result_exec->redirs;
    //     while (redir) {
    //         t_filename *tmp = redir;
    //         redir = redir->next;
    //         free(tmp->path);
    //         free(tmp);
    //     }
    //     t_arg *arg = result_exec->args;
    //     while (arg) 
	// 	{
    //         t_arg *tmp = arg;
    //         arg = arg->next;
    //         free(tmp->value);
    //         free(tmp);
    //     }
    //     free(result_exec);
    // }

    // return 0;

// }