
#include "minishell.h"

// //////////////////////////////////////
// /// TEST: PARSE               ///
// //////////////////////////////////////




// //////////////////////////////////////
// /// TEST: EXPANSION                ///
// //////////////////////////////////////

int main(int ac, char *av[], char *env[]) 
{
	(void)ac;
	(void)av;
	t_shell *content = init_shell(env);
	if (!content) 
	{
		fprintf(stderr, "Failed to initialize shell\n");
		return (1);
	}

    // Example string to test prs_exapnd_env_to_str
    char *str = "your home is $HOME, Hello $USER!!";
    char *envvar_found = strchr(str, '$'); // Assuming '$' is the first env var found
    char *expanded_str = prs_exapnd_1envvar(str, envvar_found, content);
    
    if (expanded_str) 
	{
        printf("\n|| TEST | prs_exapnd_1envvar ||\n");
		printf("Expanded string: %s\n\n", expanded_str);
        free(expanded_str);  // Don't forget to free the result!
    } else {
        printf("Failed to expand environment variable\n");
    }

    // Example token to test prs_expand_envvar_to_token
    t_token *token = token_create("Welcome $USER to $HOME", ft_strlen("Welcome $USER to $HOME"), ARG, content);

    if (prs_handle_envvar_expansion(token) == 0) 
	{  // Assuming 0 is SUCCESS
        printf("\n|| TEST | prs_handle_expansion ||\n");
		printf("Token value after expansion: %s\n\n", token->value);
        free(token->value);
        free(token);
    } else {
        printf("Failed to expand token\n");
    }

    //Clean up the shell environment
    free(content->env->id);
    free(content->env->value);
    free(content->env->env_line);
    free(content->env);
    free(content);
    return 0;
}


/////////////////////////////////////////////
/// TEST: EXPANSION HELPER- GET ENV VALUE ///
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
/// TEST: LEXING                  ///
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