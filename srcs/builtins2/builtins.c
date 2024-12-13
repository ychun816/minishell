/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:37:19 by yilin             #+#    #+#             */
/*   Updated: 2024/12/13 23:10:31 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** 42 subject for builtins:
 * - pwd with no options
 * - export with no options
 * - unset with no options
 * - env with no options or arguments
 * - exit with no options
 * - echo with option -n
 * - cd with only a relative or absolute path
 */

#include "minishell.h"

/** CHECK IS BUILTIN (bi_is_builtin)
 * builtin: echo / cd / pwd / export / unset / env / exit
*/
int	check_is_builtin(char* input_cmd)
{
	if (!input_cmd)
		return (0);
	else if (ft_strcmp(input_cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(input_cmd, "exit") == 0)
		return (2);
	else
		return (0);
}

/** EXEC BUILTIN (bi_is_builtin)
 * builtin: echo / cd / pwd / export / unset / env / exit
*/
int	exec_builtin(t_shell *content, char* input_cmd, t_arg *args)
{
	if (ft_strcmp(input_cmd, "echo") == 0)
		return (ft_echo(args));
	else if (ft_strcmp(input_cmd, "cd") == 0)
		return (ft_cd(content, args));
	else if (ft_strcmp(input_cmd, "pwd") == 0)
		return (ft_pwd(args));
	else if (ft_strcmp(input_cmd, "export") == 0)
		return (ft_export(content, args));
	else if (ft_strcmp(input_cmd, "unset") == 0)
		return (ft_unset(content, args));
	else if (ft_strcmp(input_cmd, "env") == 0)
		return (ft_env(content, args));
		// return (builtin_env(content, args));
	else if (ft_strcmp(input_cmd, "exit") == 0)
		return (ft_exit(content, args));
	else
		return (0);
}

////////// BUILTIN //////////

/** FT_ENV (with no options/args)
 * - If no argument -> print env (arrays of arrays)
 * - If there is argument -> output: env: '[input line]': No such file or directory
*/
int	ft_env(t_shell *content, t_arg *args) //t_shell *content, t_arg *args
{
	t_env	*env;
	char	*input_line;
	
	input_line = args->value;
	if (args)
	{
		error_env(input_line);
		content->exit_code = CMD_NOT_FOUND;//comment not found=127
	}
	while (env)
	{
		if (env->value)
			printf("%s\n",env->env_line);
		env = env->next;
	}
	return (SUCCESS);
}

/** PWD (no options) 
 * - If no argument -> absolute path name of the current directory that does not contain the file names
 * - pwd -L / pwd -P
 * @note
 * *(args->value) == '-' 
 * -> to check if the first character is '-' => valid option
 * -> Identify Flag Arguments
 * -> Prevent Invalid Arguments
 * (Without this check, you would attempt to compare a non-flag argument (like L) against "-L", which would produce incorrect results.)
*/
int	ft_pwd(t_arg *args)
{
	char	*cwd; //currunt working directory
	char	*option;
	
	option = args->value;
	if (args && *(args->value) == '-' && ft_strcmp(args->value, "-L") && ft_strcmp(args->value, "-P"))
	{
		error_pwd(option);
		return (2);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd) 
	{
    	perror("minishell: pwd: error retrieving current directory");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

/** ECHO (with option -n)
 * -n / -nnnnnn (followed only by character 'n') -> valid option => remove \n
 * -nP , -n-n, -nOPEK (followed by non 'n') -> invlaid
*/
int	ft_echo(t_arg *args)
{
	while (args && !check_echo_nflag(args->value)) //-n flag
		args = args->next; //skip -n flag
	while (args)
	{
		printf("%s", args->value);
		if (args->next)
			printf(" ");
		args = args->next;
	}
	// Print a newline if no -n flags were encountered
	if (args)// This will be true if there are any remaining arguments
		printf("\n");
	return (SUCCESS);
}

int	check_echo_nflag(char *flag)
{
	int	i;

	i = 0;
	if (!ft_strncmp(flag, "-n", 2))
		i += 2;
	else
		return (1); //NO, INVALID
	if (flag[i + 1] != 'n')
		return (1); //NO, INVALID
	return (0);
}

/** EXIT (no options) 
 * exit [n]
 * - If more than 1 argument -> error: too many arguments
 * - if 1 argument & is a valid exit code (it should be a number) -> exit with exit code
 * - If 1 argument & is invalid -> error_exit()
 * In the end, close and free everything
*/
int	ft_exit(t_shell *content, t_arg *args)
{
	int	exit_code;

	if (args && args->next && !check_exitcode(args->value))
		return (ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO), 1);
	exit_code = 0;
	if (args && !check_exitcode(args->value))
		exit_code = ft_atoi(args->value);  // If it's a valid exit code, convert the argument to an integer
	else if (args && check_exitcode(args->value))
	{
		error_exit(args->value);
		exit_code = 2;
	}
	ft_close(content);
	free_all_shell(content);
	return (SUCCESS);	
}

/** CHECK_EXIT_CODE 
 * 
 * @note
 * char *char tmp : point to the provided value -> Preserving the Original Pointer
 * 
*/
int	check_exitcode(char	*input_line)
{
	char	*tmp;
	long	exit_nb;

	tmp = input_line;
	
	// If the value starts with '+' or '-', skip the sign
	if (((*tmp) == '+' || (*tmp) == '-') && *(tmp + 1))
		tmp++;
	// Iterate through each character in the value to check if it's a digit
	while (*tmp)
	{
		if (!ft_isdigit(*tmp))//not digit
			return (1);
		tmp++;	
	}
	// Convert the value to a long integer using ft_atol
	exit_nb= ft_atol(input_line);
	// Check if the number overflows or underflows the range of valid exit codes
	if ((exit_nb > 0 && LONG_MAX / exit_nb < 1) 
		|| (exit_nb < 0 && LONG_MIN / ft_atol(input_line) < 1))
		return (1);
	 return (0);// Return 0 if the value is a valid exit code
}



/** CD (relative or absolute path)
 * 1. Get the number of arguments passed to 'cd'
 * 2. If more than 1 args -> "error: too many arguments"
 * 3. Get the current working directory and store it in variable, if fail-> perror
 * 4. Get the 'HOME' environment variable
 * 5. 
 * 
 * 
*/
int	ft_cd(t_shell *content, t_arg *args)
{
	int	args_nb;
	char	*cwd;
	t_env	*home;
	
	args_nb = ft_arg_lstsize(args);
	if (args_nb > 1)
		return (ft_putstr_fd("minishell: cd: too many arguments\n",
				STDERR_FILENO), 1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell: cd: error retrieving current directory");
	home = get_env("HOME", content->env);
	//take care of "--"
	if ((!args_nb || !ft_strcmp(args->value, "--")) && home && home->value)
		chdir(home->value);
	else if ((!args_nb || !ft_strcmp(args->value, "--")) && (!home || !home->value))
		return (error_cd(errno, "HOME"), free(cwd), 1);
	else if (chdir(args->value) < 0)
	{
		error_cd(errno, args->value);
		return (free(cwd), 1);
	}
	if (update_pwd(content, cwd))
		return (free(cwd), 1);		
	return (free(cwd), SUCCESS);
}
/** UPDATE PWD 
 * Updates the PWD (current working directory) and OLDPWD (previous working directory) environment variables. * 
 * 
 * @param
 * - t_shell *content: A pointer to the shell's context structure containing the environment.
 * - t_env *old_pwd / pwd: interact wiht the environment
 * - char *current_cwd: Directly holds the raw directory path returned by getcwd
 * - char *pwd_path
 * - char *oldpwd_path
 * 
 * (1) Get the current working directory and store it in new_cwd
 * (2) Retrieve the OLDPWD and PWD environment variables from the environment list.
 * (3) If PWD valid && current_cwd (current working directory) is valid, 
 * -> Create a new string in the format "PWD=<current_working_directory>".
 * -> Check for memory allocation failure or failure to add the variable to the environment.
 * -> Free the temporary string
 * (4) If OLDPWD valid && OLDPWD value exists/provided
 * -> Create a new string "OLDPWD=<previous_working_directory>". -> Check if fail -> Cleanup
 * 
*/
int update_pwd(t_shell *content, char *oldpwd_value)
{
	t_env	*old_pwd;
	t_env	*pwd;
	char	*current_cwd;
	char	*pwd_path;
	char	*oldpwd_path;

	current_cwd = getcwd(NULL, 0);
	
	old_pwd = get_env("OLDPWD", content->env);
	pwd = get_env("PWD", content->env);
	
	if (pwd && current_cwd)
	{
		pwd_path = ft_strjoin("PWD=", current_cwd);
		if (!pwd_path || add_envvar( pwd_path, &content->env) != 0)
			return (free(current_cwd), FAILURE);
		free (pwd_path);
	}
	if (old_pwd && oldpwd_value)
	{
		oldpwd_path = ft_strjoin("OLDPWD=", oldpwd_value);
		if (!oldpwd_path || add_envvar( oldpwd_path, &content->env) != 0)
			return (free(current_cwd), FAILURE);
		free(oldpwd_path);
	}
	free(current_cwd);
	return (SUCCESS);//0
}

/** ADD ENVVAR
 * Adds or updates an environment variable
 * Ensures that the variable's name and value are valid
 * Creates a new environment entry or updates an existing one.
 * 
 * @param
 * char	*arg_id;         // The name/ID of the environment variable.
 * char	*arg_value_dup;  // Duplicate of the value string.
 * t_env	*tmp;        // Temporary pointer to find or create the variable.
 * 
 * (1) Extract the variable name(id) from the input string. -> Check if valid
 * (2) Duplicate the entire "KEY=VALUE" string.
 * (3) Check if the variable already exists in the environment.
 * -> If fail OR env not exist => Create new one (initialize + add end-node)
 * -> If env exit => Update the existing variable.
 * 
*/
int	add_envvar(char *env_line, t_env **env_head)
{
	char	*env_id;
	char	*dup_envline;
	t_env	*env;
	
	env_id = get_env_id(env_line);
	if (!env_id || !check_envid_valid(env_id))
		return (free(env_id), error_export(env_line));

	dup_envline = ft_strdup(env_line);
	if (!dup_envline)
		return (free(env_id), FAILURE);//1

	env = get_env(env_line, *env_head);
	if (!env)
	{
		env = env_create(ft_strdup(env_id), ft_strdup(env_line), env_line);
		if (!env)
			return (free(env_line), free(env_id), 1);
		env_add_back(env_head, env);	
	}
	else
	{
		free (env_id);
		update_envvar(env, dup_envline);
	}
	return (SUCCESS);//0
}

/** CHECK ENVID VALID 
 * (1) Check if env_id empty
 * (2) Check if env_id first character alpha or not '_'
 * (3) Then loop to check if env_id rest of character numeric
 * 
 * @note
 * - Valid: "HOME", "USER_NAME", "_PWD".
 * - Invalid: "123ABC", "$PATH", "!VAR" (
 * because the first character is not a letter or underscore).
 * 
*/
int	check_envid_valid(char *env_id)
{
	int	i;

	i = 0;
	if (env_id[i] == '\0')
    	return (0);
	if (ft_isalpha(env_id[i]) == 0  && env_id[i] != '_')
        return (0);
	i++;
	while (env_id[i])
	{
		if (ft_isalnum(env_id[i]) == 0 && env_id[i] != '_')
			return (0);//invalid
		i++;
	}
	return (1);//env_id is valid
}


/** UPDATE ENV VAR 
 * Updates an existing environment variable node's value and its full line representation.
 * It frees old memory for the value and line and assigns new ones.
 * 
 * (1) Retrieve the value associated with the raw environment string
 * (2) If no valid value was found (arg_value is NULL), free the raw string -> exit
 * (3) If the 'node (update_env)' has an existing value => free the old value memory.
 * (4) If the 'node  (update_env)' has an existing environment line (raw string) => free old env_line
 * (5) Assign the new value and environment line to the 'node' (update_env) structure.
 * 
*/
void	update_envvar(t_env *update_env, char *env_line)
{
	char	*env_value;

	env_value = get_env_value(env_line);
	
	if (!env_value)
	{
		free(env_value);
		return ;
	}
	if (update_env->value)
		free(update_env->value);
	if (update_env->env_line)
		free(update_env->env_line);
	update_env->value = env_value;
	update_env->env_line = env_line;
}

////////////////////////////////////////////////////

/** EXPORT (with no options)
 * Handling casess: 
 * 1. export
 * 2. export ENV_VARIABLE=VALUE (-> add env variable)
 * 3. export ENV_VARIABLE1=VALUE1 ENV_VARIABLE2=VALUE2 ENV_VARIABLE3=VALUE3
 * 
 * - If no arg -> print env according to alphabetic order
 * - If there's arg
*/
int	ft_export(t_shell *content, t_arg *args)
{	
	if (!args)
	{
		export_print_ordered_env(content->env);
		return (FAILURE);//1
	}
	else
	{
		while (args)
		{
			if (add_envvar(args->value, &(content->env)) != 0)
				return (FAILURE);//1
			args = args->next;
		}
	}
	return (SUCCESS);
}


/** export_print_ordered_env 
 * (1) transform env list to env arrays
 * (2) sort env arrays by alphabetic orders
 * (3) print arrays
 * 
*/
void	export_print_ordered_env(t_env *env)
{
	char	**env_arrs;
	char	**sorted_envarrs;
	char	*tmp_value;
	char	*tmp_id;
	int	i;

	i = 0;
	env_arrs = env_format(env);
	if (!env_arrs)
		return ;
	sorted_envarrs = sort_env_arrs(env_arrs);
	
	//print env arrs  //export USER="lin"
	while (sorted_envarrs[i])
	{
		printf("export ");//print export
		//get env_var
		tmp_value = ft_strchr(sorted_envarrs[i], "=") + 1;
		if (!tmp_value)
			printf ("%s\n", sorted_envarrs[i]);
		else
		{
			tmp_id = get_env_id(sorted_envarrs[i]);
			printf("%s = %s\n", tmp_id, tmp_value);
			free(tmp_id);
		}
		free(sorted_envarrs);	
		i++;
	}
	free(env_arrs);  // Free the original environment array
    free(sorted_envarrs);  // Free the sorted array
}

/** sort_env_arrs */
char **sort_env_arrs(char **env_arrs)
{
	char	**result;
	int	i;
	int	j;
	int	len;
	char	*swap;
	
	len = 0;
	while (env_arrs[len])
		len++;
	result = malloc(sizeof(char*) * (len + 1));//save space for NULL
	if (!result)
		return (NULL);

	 //copy env_arrs to reuslt
	i = 0;
	while (i < len)
	{
		result[i] = ft_strdup(env_arrs[i]);
		i++;
	}
	result[i] = NULL; 

	//sorting		
	i = 0;
	while(result[i])
	{
		j = 1;
		while (result[i + j])
		{
			if (ft_strcmp(result[i], result[j]) > 0)
			{
				swap = result[i];
				result[i] = result[j];
				result[j] = swap;
			}
			j++;
		}
		i++;
	}
	return (result);
}



/** UNSET (with no options)
 * unset VAR1 VAR2 VAR3
 * 
*/
int	ft_unset(t_shell *content, t_arg *args)
{
	if (!args)
		return (SUCCESS);//0
	else
	{
		while (args)
		{
			if (delete_env_var(args->value, &(content->env)) != 0)
				return (FAILURE);			
			args = args->next;
		}
	}
	return (SUCCESS);
}

/** delete env var */

