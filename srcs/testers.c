
#include "minishell.h"
#include <stdio.h>

// //////////////////////////////////////
// /// TEST: PARSING                  /// ///CHECKED!!
// //////////////////////////////////////
// int main(int ac, char *av[], char *env[])
// {
//     (void)ac;
// 	(void)av;
// 	t_shell *content = init_shell(env);
// 	if (!content) 
// 	{
// 		fprintf(stderr, "Failed to initialize shell\n");
// 		return (1);
// 	}

	///// /** 1. TEST: remove NULL token */ ///// 
	// printf("\n--- Test 1: Removing NULL Tokens ---\n");
	// t_token *head = NULL;
    // test_print_tokens(head);
    // // Create a list with valid and NULL values
    // test_token_append(&head, "echo", 4, STR, content);
    // test_token_append(&head, NULL, 0, STR, content); // NULL value token
    // test_token_append(&head, "Hello", 5, STR, content);
    // test_token_append(&head, NULL, 0, STR, content); // NULL value token
    // test_token_append(&head, "World", 5, STR, content);
    // printf("Before removing NULL nodes:\n");
    // test_print_tokens(head);
    // // Call the function to remove NULL value nodes
    // prs_remove_node_null(&head);
    // printf("After removing NULL nodes:\n");
    // test_print_tokens(head);
    // // Free allocated memory
    // while (head) 
	// {
    //     t_token *tmp = head->next;
    //     free(head->value);
    //     free(head);
    //     head = tmp;
    // }

/////////////////////////////////////////////////////////////////////////////////////

	///// /** 2. TEST: prs_check_allnodes_null */ /////
    // Test 1: List with no NULL nodes
    // printf("--- Test 1: List with no NULL nodes ---\n");
    // t_token *head1 = NULL;
    // test_token_append(&head1, "Hello", 5, STR, content);
    // test_token_append(&head1, "World", 5, STR, content);

    // test_print_tokens(head1);

    // int result1 = prs_check_allnodes_null(head1);
    // printf("Result: %d\n", result1); // Should print 0 because there are non-NULL tokens

    // // Clean up
    // while (head1)
    // {
    //     t_token *tmp = head1->next;
    //     free(head1->value);
    //     free(head1);
    //     head1 = tmp;
    // }

    // // Test 2: List with all NULL nodes
    // printf("\n--- Test 2: List with all NULL nodes ---\n");
    // t_token *head2 = NULL;
    // test_token_append(&head2, NULL, 0, STR, content);
    // test_token_append(&head2, NULL, 0, STR, content);
    
    // int result2 = prs_check_allnodes_null(head2);
    // printf("Result: %d\n", result2); // Should print 1 because all tokens are NULL

    // // Test 3: List with mixed NULL and non-NULL nodes
    // printf("\n--- Test 3: List with mixed NULL and non-NULL nodes ---\n");
    // t_token *head3 = NULL;
    // test_token_append(&head3, "First", 5, STR, content);
    // test_token_append(&head3, NULL, 0, STR, content);
    // test_token_append(&head3, "Third", 5, STR, content);

    // test_print_tokens(head3);

    // int result3 = prs_check_allnodes_null(head3);
    // printf("Result: %d\n", result3); // Should print 0 because there is at least one non-NULL token

    // // Clean up
    // while (head3)
    // {
    //     t_token *tmp = head3->next;
    //     free(head3->value);
    //     free(head3);
    //     head3 = tmp;
    // }

/////////////////////////////////////////////////////////////////////////////////////

	///// /** 3. TEST: prs_handle_redir */ /////
	/* TEST CASE

	"echo Hello > output.txt"
	"cat < input.txt"
	"ls | grep txt"
	"cat < input.txt > output.txt"
	"grep \"search\" << EOF"
	"echo \"Hello\" >> output.txt"
	"echo \"Hello\" > output.txt | cat"
	
	INVALID
	"cat <"
	command <
	command >
	command <<
	command >> >>
	command > < 
	command > file | command2
	command | command2 > file
	| command
	command |
	command1 > file command2
	command > file with spaces
	command >> file$>name
	> outputfile
	command | | command2
	command < file | command2
	command1 < command2

	echo > file
	command > file < file2
	> file
	*/

    // Step 1: Initialize the shell context
    // t_shell shell = {0};

    // // Step 2: Tokenize the input string provided in argv[1]
    // char *input = av[1];
    // t_token *tokens = test_tokenize_input(input, &shell);

    // // Step 3: Print tokens before processing
    // printf("\n--- BEFORE prs_handle_redir: ---\n");
    // test_print_tokens(tokens);

    // // Step 4: Process redirection using the prs_handle_redir function
    // int result = prs_handle_redir(tokens);

    // // Step 5: Print the result of redirection handling and the updated tokens
    // if (result == SUCCESS)
    //     printf("\n=> :) SUCCESS-Redirection handling! <=\n");
    // else
    //     printf("\n=> :( FAIL-Redirection handling! <=\n");

    // printf("\n--- AFTER prs_handle_redir: ---\n");
    // test_print_tokens(tokens);

    // // Step 6: Free allocated memory (you should implement a function to do this in production)
    // t_token *tmp;
    // while (tokens)
    // {
    //     tmp = tokens;
    //     tokens = tokens->next;
    //     free(tmp->value);
    //     free(tmp);
    // }

/////////////////////////////////////////////////////////////////////////////////////

	///// /** 4. TEST: prs_handle_cmd */ /////
	/*
	echo Hello | cat file.txt
	echo Hello
	ls -l | grep txt
	echo Hello > output.txt
	cat < input.txt  /////////////////TO CHECK HERE
	grep -r "search_term" | sort
	echo "data" >> output.log
	cat < input.txt > output.txt
	cat << EOF
	echo 'single quoted' > output.txt


	*/
	// Initialize the shell context (for testing, we set it to NULL)
	// t_shell *content = NULL;

	// Tokenize the input (simulate simple tokenization)
	// In a real scenario, you would tokenize based on spaces, operators, etc.
	// t_token *head = NULL, *last_token = NULL;
	// char *input = av[1];
	// char *token_str = strtok(input, " "); // Assuming space as the delimiter

	// while (token_str != NULL)
	// {
	// 	t_token_type type = STR; // Default type for words
	// 	if (strcmp(token_str, "|") == 0)
	// 		type = PIPE;
	// 	// Add more checks here for other operators if necessary (e.g., INFILE, OUTFILE)

	// 	// Create a new token and add it to the list
	// 	t_token *new_token = token_create(token_str, strlen(token_str), type, content);
	// 	if (last_token)
	// 		last_token->next = new_token;
	// 	else
	// 		head = new_token; // Set the head if it's the first token
	// 	last_token = new_token;

	// 	// Get the next token
	// 	token_str = strtok(NULL, " ");
	// }

	// // Print tokens before calling prs_handle_cmd
	// printf("Tokens before prs_handle_cmd:\n");
	// test_print_tokens(head);

	// // Call the function to process the tokens
	// prs_handle_cmd(head);

	// // Print tokens after calling prs_handle_cmd
	// printf("\nTokens after prs_handle_cmd:\n");
	// test_print_tokens(head);

	// // Free memory (for testing purposes)
	// t_token *current = head;
	// t_token *next_token;
	// while (current)
	// {
	// 	next_token = current->next;
	// 	free(current->value);
	// 	free(current);
	// 	current = next_token;
	// }

///////////////////////////////////////////////////////

	///// /** 5. TEST: prs_init_heredoc */ /////
	// Test: Setting up a token list with a HEREDOC token
    // t_token *head = (t_token *)malloc(sizeof(t_token));
    // t_token *second = (t_token *)malloc(sizeof(t_token));

    // // Initialize tokens
    // head->value = strdup("<<");  // HEREDOC token
    // head->type = HEREDOC;
    // head->next = second;

    // // Use the delimiter provided via av[1] (the first argument passed to the program)
    // second->value = strdup(av[1]);  // The delimiter for the heredoc
    // second->type = STR;  // Not a HEREDOC itself, but the content
    // second->next = NULL;

    // // Simulate processing the tokens
    // printf("Processing heredoc with delimiter: %s\n", av[1]);
    // int result = prs_handle_heredoc(head);

    // // Check the result
    // if (result == SUCCESS)//0
    //     printf("Heredoc handled successfully!\n");
    // else
    //     printf("Failed to handle heredoc.\n");

    // // Clean up
    // free(head->value);
    // free(head);
    // free(second->value);
    // free(second);

///////////////////////////////////////////////////////

	///// /* 6. TEST: unlink */ /////
    //Step 1: Create test files
    // Step 1: Create test files
//     FILE *file1 = fopen("test_file1.txt", "w");
//     FILE *file2 = fopen("test_file2.txt", "w");
//     if (file1) fclose(file1);
//     if (file2) fclose(file2);

//     // Step 2: Create linked list of tokens
//     t_token *head = token_create("test_file1.txt", strlen("test_file1.txt"), NON_HEREDOC, NULL);
//     head->next = token_create("test_file2.txt", strlen("test_file2.txt"), NON_HEREDOC, NULL);

//     // Step 3: Call prs_unlink_error to delete files
//     printf("Before prs_unlink_error:\n");
//     printf("File 1 exists: %s\n", access("test_file1.txt", F_OK) == 0 ? "Yes" : "No");
//     printf("File 2 exists: %s\n", access("test_file2.txt", F_OK) == 0 ? "Yes" : "No");

//     prs_unlink_error(head);

//     // Step 4: Verify files are deleted
//     printf("After prs_unlink_error:\n");
//     printf("File 1 exists: %s\n", access("test_file1.txt", F_OK) == 0 ? "Yes" : "No");
//     printf("File 2 exists: %s\n", access("test_file2.txt", F_OK) == 0 ? "Yes" : "No");

//     // Step 5: Clean up
//     token_free(head);


// 	return 0;
// }


// //////////////////////////////////////
// /// TEST: PARSING- QUOTES          ///   ///CHECKED!!
// //////////////////////////////////////
// int main(int ac, char *av[], char *env[]) 
// {
// 	(void)ac;
// 	// (void)av;
// 	t_shell *content = init_shell(env);
// 	if (!content) 
// 	{
// 		fprintf(stderr, "Failed to initialize shell\n");
// 		return (1);
// 	}
    // Example input strings
    // char *input1 = "Hello 'world' \"test\"";
    // char *input2 = "'single quote string'";
    // char *input3 = "\"double quote string\"";
    // char *input4 = "no quotes here";
	// char *input5 = "invalid'   ' a'quote";
	/*

	// AV[] TEST INPUT:
	1. 
	"Hello 'world\" test\""
	Input: Hello 'world" test"
	Quotes valid: No | int value: 1
	
	2.
	"\"unbalanced double quote"
	Input: "unbalanced double quote
	Quotes valid: No | int value: 1

	3.
	"'mixed quotes\" test"
	Input: 'mixed quotes" test
	Quotes valid: No | int value: 1

	4.
	"Hello 'world\" test"
	Input: Hello 'world" test
	Quotes valid: No | int value: 1

	5.
	""
	Input: 
	Quotes valid: Yes | int value: 0

	6.
	"'"
	Input: '
	Quotes valid: No | int value: 1

	7.
	"\""
	Input: "
	Quotes valid: No | int value: 1
	*/
    //////** 1 TEST: `prs_check_quotes_valid` */ /////////
    // printf("\n=== TEST1: prs_check_quotes_valid ===\n");
    // t_token *token1 = token_create(input1, strlen(input1), STR, content);
    // printf("Input1: %s\n", token1->value);
    // int valid = prs_check_quotes_valid(token1);
    // printf("Quotes valid: %s | int value: %d\n\n", valid == SUCCESS ? "Yes" : "No", valid);

    // printf("\n=== TEST2: prs_check_quotes_valid ===\n");
    // t_token *token2 = token_create(input2, strlen(input2), STR, content);
    // printf("Input2: %s\n", token2->value);
    // int valid2 = prs_check_quotes_valid(token2);
    // printf("Quotes valid: %s | int value: %d\n\n", valid2 == SUCCESS ? "Yes" : "No", valid2);

    // printf("\n=== TEST3: prs_check_quotes_valid ===\n");
    // t_token *token3 = token_create(input3, strlen(input3), STR, content);
    // printf("Input3: %s\n", token3->value);
    // int valid3 = prs_check_quotes_valid(token3);
    // printf("Quotes valid: %s | int value: %d\n\n", valid3 == SUCCESS ? "Yes" : "No", valid3);

    // printf("\n=== TEST4: prs_check_quotes_valid ===\n");
    // t_token *token4 = token_create(input4, strlen(input4), STR, content);
    // printf("Input4: %s\n", token4->value);
    // int valid4 = prs_check_quotes_valid(token4);
    // printf("Quotes valid: %s | int value: %d\n\n", valid4 == SUCCESS ? "Yes" : "No", valid4);

    // printf("\n=== TEST5: prs_check_quotes_valid ===\n");
    // t_token *token5 = token_create(input5, strlen(input5), STR, content);
    // printf("Input5: %s\n", token5->value);
    // int valid5 = prs_check_quotes_valid(token5);
    // printf("Quotes valid: %s | int value: %d\n\n", valid5 == SUCCESS ? "Yes" : "No", valid5);
    
	// ****Use input from command-line arguments****
    // printf("\n=== TEST: prs_check_quotes_valid ===\n");
    // char *input = av[1]; // First argument is the input string
    // printf("Input: %s\n", input);
    // // Create a token from the input string
    // t_token *token = token_create(input, strlen(input), STR, content);
    // // Check the validity of quotes in the token
    // int valid = prs_check_quotes_valid(token);
    // printf("Quotes valid: %s | int value: %d\n", valid == SUCCESS ? "Yes" : "No", valid);
    // // Free the allocated token
    // free(token->value);
    // free(token);
    // return 0;

/////////////////////////////////////////////////////////////////////////////////////

   	//// /** 2. TEST: `prs_get_quoted_str` */ /////////
    // printf("\n=== TEST: prs_get_quoted_str ===\n");
    // char *quoted_input = "'quoted string'";
    // t_token *quoted_token = prs_get_quoted_str(quoted_input, '\'', content);
    // printf("Quoted input: %s\n", quoted_input);
    // printf("Extracted token: %s\n\n", quoted_token->value);
	// free(quoted_token->value);
    // free(quoted_token);
	// return 0;
	
	// ****Use input from command-line arguments****
	/* TEST INPUTS
	1. 
	"'hello world'" "'"
	Quoted input: 'hello world'
	Extracted token: hello world

	2. 
	"\"hello world\"" "\""
	Quoted input: "hello world"
	Extracted token: hello world
	
	3.
	"prefix 'nested quote' suffix" "'"
	Quoted input: prefix 'nested quote' suffix
	Extracted token: nested quote

	EDGE CASE
	4. unclosed quotes
	"'hello world" "'"
	Quoted input: 'hello world
	Error: Failed to extract quoted string from input.

	5. No Quotes
	"no quotes here" "'"
	Quoted input: no quotes here
	Error: Failed to extract quoted string from input.

	6. Empty Input String
	"" "'"
	Quoted input: 
	Error: Failed to extract quoted string from input.

	7. Mismatched Quote Character
	"'hello world'" "\""
	Quoted input: 'hello world'
	Error: Failed to extract quoted string from input.
	
	8. Quote Character Without Content
	"''" "'"
	Quoted input: ''
	Extracted token: 
	*/

    // printf("\n=== TEST: prs_get_quoted_str ===\n");
    // t_token *quoted_token = prs_get_quoted_str(av[1], av[2][0], content);
    // printf("Quoted input: %s\n", av[1]);
    // printf("Extracted token: %s\n\n", quoted_token->value);
	// free(quoted_token->value);
    // free(quoted_token);
	// return 0;

/////////////////////////////////////////////////////////////////////////////////////

    //////** 3. TEST:  Test `ft_rogue_len` */ /////
	/* TEST INPUTS
	1."hello world"
	Input: hello world
	Result: 11
	
	2. "hello world'"
	Input: hello world'
	Result: 11
	
	3. "hello'world"
	Input: hello'world
	Result: 5

	EDGE CASE
	4. Empty String
	""
	Input: 
	Result: 0

	5. String Starting With a Quote 
	"'hello world"
	Input: 'hello world
	Result: 0

	6. String With Multiple Quotes
	"hello'world\"test"
	Input: hello'world"test
	Result: 5

	7. String Consisting Only of Quotes
	"'\""
	Input: '"
	Result: 0

	*/
	// printf("=\n== TEST: ft_rogue_len ===\n");
    // printf("Rogue input: %s\n", av[1]);
    // printf("Rogue length: %d\n\n",  ft_rogue_len(av[1]));

/////////////////////////////////////////////////////////////////////////////////////

    ///// /** 4. TEST: `prs_quotes_to_tokens` */ /////
	/* TEST INPUT CASE
	1. "'hello world'"
	Input: 'hello world'
	Token: hello world (from single quote)

	2. 'hello world'
	Input: "hello world"
	Token: hello world (from double quote)

	3. "hello 'world' \"test\""
	Input: hello 'world' "test"
	Tokens: 
	- hello
	- world (from single quote)
	- test (from double quote)

	EDGE CASE
	4. "'"
	Input: '
	Token: (empty, invalid string)
	
	5. "\""
	Input: "
	Token: (empty, invalid string)

	6. ""
	Input: (empty)
	Token: (empty, no token created)

	7. "\"hello\" 'world' \"test\""
	Input: "hello" 'world' "test"
	Tokens:
	- hello (from double quote)
	- world (from single quote)
	- test (from double quote)

	*/
    // printf("\n=== TEST: prs_quotes_to_tokens ===\n");
    // // char *mixed_input = "Hello 'world' and \"universe\"";
    // t_token *mixed_tokens = prs_quotes_to_tokens(av[1], content);
    // printf("Input: %s\n", av[1]);
    // printf("Tokens:\n");
    // t_token *current = mixed_tokens;
    // while (current)
    // {
    //     printf("  Token: %s (type: %d)\n", current->value, current->type);
    //     current = current->next;
    // }
    // printf("\n");

/////////////////////////////////////////////////////////////////////////////////////

    //// /** 5. TEST: Test `prs_handle_quotes_n_expand_env` */ ////
	/* TEST INPUTS
	
	1. "'hello world'"
	Input: 'hello world'
	Expanded and handled quotes: hello world
	2. "\"Hello $USER\""
	Input: "Hello $USER"
	Expanded and handled quotes: Hello <username>
	3. "prefix 'quoted' $HOME"
	Input: prefix 'quoted' $HOME
	Expanded and handled quotes: prefix quoted /path/to/home
	4. "hello 'world"
	Input: hello 'world
	Expanded and handled quotes: <error or partial handling>
	5. ""
	Input: 
	Expanded and handled quotes: 
	6. "$USER"
	Input: $USER
	Expanded and handled quotes: <username>

	7. "'$USER'"
	Input: '$USER'
	Expanded and handled quotes: $USER

	8. "    "
	Input:     
	Expanded and handled quotes:   

	9. 
	"'hello \"world\"'"
	Input: 'hello "world"'
	Expanded and handled quotes: hello "world"

	COMPLEX CASES
	10. Multiple Quotes and Variables
	"prefix 'quoted $USER' \"suffix $HOME\""
	Input: prefix 'quoted $USER' "suffix $HOME"
	Expanded and handled quotes: prefix quoted $USER suffix /path/to/home

	11. 
	"prefix $HOME \"escaped \\\"quote\\\"\" $USER"
	"prefix \"escaped \\\"quote\\\"\""

	*/	
// 	printf("\n=== TEST: prs_handle_quotes_n_expand_env ===\n");
//     t_token *env_input = token_create(av[1], strlen(av[1]), STR, content);
//     printf("Input: %s\n", av[1]);// printf("Input: %s\n", env_input->value);
//     prs_handle_quotes_n_expand_env(env_input);
//     printf("Expanded and handled quotes: %s\n\n", env_input->value);

//     // Free memory
//     // token_free(token1);
//     // token_free(quoted_token);
//     // token_free(mixed_tokens);
//     // token_free(env_input);

//     return 0;
// }

// //////////////////////////////////////
// /// TEST: EXPANSION                ///  ///CHECKED!!
// //////////////////////////////////////

// int main(int ac, char *av[], char *env[]) 
// {
// 	(void)ac;
// 	(void)av;
// 	t_shell *content = init_shell(env);
// 	if (!content) 
// 	{
// 		fprintf(stderr, "Failed to initialize shell\n");
// 		return (1);
// 	}

//     // Example string to test prs_exapnd_env_to_str
//     char *str = "your home is $HOME, Hello $USER!!";
//     char *envvar_found = strchr(str, '$'); // Assuming '$' is the first env var found
//     char *expanded_str = prs_exapnd_1envvar(str, envvar_found, content);
    
//     if (expanded_str) 
// 	{
//         printf("\n|| TEST | prs_exapnd_1envvar ||\n");
// 		printf("Expanded string: %s\n\n", expanded_str);
//         free(expanded_str);  // Don't forget to free the result!
//     } else {
//         printf("Failed to expand environment variable\n");
//     }

//     // Example token to test prs_expand_envvar_to_token
//     t_token *token = token_create("Welcome $USER to $HOME", ft_strlen("Welcome $USER to $HOME"), ARG, content);

//     if (prs_handle_envvar_expansion(token) == 0) 
// 	{  // Assuming 0 is SUCCESS
//         printf("\n|| TEST | prs_handle_expansion ||\n");
// 		printf("Token value after expansion: %s\n\n", token->value);
//         free(token->value);
//         free(token);
//     } else {
//         printf("Failed to expand token\n");
//     }

//     //Clean up the shell environment
//     free(content->env->id);
// free(content->env->value);
//     free(content->env->env_line);
//     free(content->env);
//     free(content);
//     return 0;
// }

/////////////////////////////////////////////
/// TEST: EXPANSION HELPER- GET ENV VALUE ///  ///CHECKED!!
////////////////////////////////////////////
// int main(int ac, char *av[], char *env[]) 
// {
// 	(void)ac;
// 	(void)av;
// 	t_shell *content = init_shell(env);
// 	if (!content) 
// 	{
// 		fprintf(stderr, "Failed to initialize shell\n");
// 		return (1);
// 	}
// 	char *result;
// 	char *result2;
// 	char *result3;
// 	char *result4;
// 	char *result5;
// 	char *result6;

// 	// Test case 1: $HOME
// 	result = get_envvar_value("HOME", content);
// 	printf("Test $HOME: %s\n", result ? result : "NULL");
// 	free(result);

// 	// Test case 2: $USER
// 	result2 = get_envvar_value("USER", content);
// 	printf("Test2 $USER: %s\n", result2 ? result2 : "NULL");
// 	free(result2);

// 	// Test case 3: $PATH
// 	result3 = get_envvar_value("PATH", content);
// 	printf("Test3 $PATH: %s\n", result3 ? result3 : "NULL");
// 	free(result3);

// 	// Test case 4: $?
// 	result4 = get_envvar_value("?", content);
// 	printf("Test4 $? (exit code): %s\n", result4 ? result4 : "NULL");
// 	free(result4);

// 	// Test case 5: $$
// 	result5 = get_envvar_value("$", content);
// 	printf("Test5 $$ (process ID): %s\n", result5 ? result5 : "NULL");
// 	free(result5);

// 	// Test case 6: Undefined variable
// 	result6 = get_envvar_value("UNDEFINED", content);
// 	printf("Test6 $UNDEFINED: %s\n", result6 ? result6 : "NULL");
// 	free(result6);
// 	return 0;
// }


//////////////////////////////////////
/// TEST: LEXING                  ///  ///CHECKED!!
/////////////////////////////////////

// void print_tokens(t_token *token) 
// {
//     t_token *current;

// 	current = token;
// 	while (current) 
// 	{
//         // Print the value, length, and type of each token
//         printf("Token Value: %s\n", current->value);
//         printf("Token Length: %zu\n", ft_strlen(current->value)); // Use strlen to calculate the length //zu: size_t
//         printf("Token Type: %d\n\n", current->type);
//         current = current->next;
//     }
// }

// int main(void) 
// {
//     // Assuming `content` is some shell-related structure
//     void *content = NULL;  // Set to NULL for simplicity in this example

//     // Test input string for lexing
//     char *test_input = "echo 'hello world' > output.txt | cat";  // Test string to tokenize

//     // Test lex_tokenize_each_wd
//     printf("=== Testing lex_tokenize_each_wd ===\n\n");

//     // Example test cases for `lex_tokenize_each_wd`
//     char *test_strings[] = 
// 	{
//         "<",				 // INFILE 0
// 		">",                 // Redirection OUTFILE 1
// 		"<<",				 // HEREDOC 2
// 		">>",				 // APPEND
//         "|",                 // Pipe
//         "echo",              // Command
//         "'hello world'",     // Quoted string
//         "output.txt",        // Filename
//         "cat",               // Command
//         NULL
//     };

//     // Test lex_tokenize_each_wd with each input string
//     for (int i = 0; test_strings[i] != NULL; i++) {
//         printf("Testing lex_tokenize_each_wd with input: \"%s\"\n", test_strings[i]);
//         t_token *token = lex_strtok(test_strings[i], content);
//         if (token) {
//             print_tokens(token);
//             // Free allocated memory if necessary
//             free(token->value);
//             free(token);
//         }
//         printf("\n");
//     }

//     // Test lexing function (which calls lex_tokenize_each_wd)
//     printf("=== Testing lexing ===\n\n");
//     t_token *tokens = lexer(content, test_input);  // Pass the test input to lexing function
//     print_tokens(tokens);  // Print all tokens

//     // Free memory used by the lexing function
//     if (tokens) {
//         tok_free(tokens);  // Assuming token_free is implemented to free the list of tokens
//     }
//     return 0;
// }