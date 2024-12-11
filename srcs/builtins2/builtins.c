/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:37:19 by yilin             #+#    #+#             */
/*   Updated: 2024/12/11 20:01:07 by yilin            ###   ########.fr       */
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
 * updates the PWD (current working directory) and OLDPWD (previous working directory) 
 * 
 * @param
 * - t_shell *content: A pointer to the shell's context structure containing the environment.
 * - char *new_pwd: Directly holds the raw directory path returned by getcwd
 * - t_env *old_pwd / pwd: interact wiht the environment
 * 
 * (1) Get the current working directory and store it in new_cwd
 * (2) Retrieve the OLDPWD and PWD environment variables from the environment list.
 * (3) If PWD exists and new_cwd (current working directory) is valid, 
 * -> Create a new string in the format "PWD=<current_working_directory>".
 * -> Check for memory allocation failure or failure to add the variable to the environment.
 * -> Free the temporary string
 * 
*/
int update_pwd(t_shell *content, char *oldpwd_value)
{
	t_env	*old_pwd;
	t_env	*pwd;
	char	*new_cwd;
	char	*tmp_oldpwd;          // Temporary string for setting OLDPWD.
	char	*tmp_pwd;         // Temporary string for setting PWD.

	new_cwd = getcwd(NULL, 0);
	
	old_pwd = get_env("OLDPWD", content->env);
	pwd = get_env("PWD", content->env);
	
	if (pwd && new_cwd)
	{
		
	}

	
}

/** ADD VAR   */ //TO DO!! 
int	bi_add_var(char *value, t_env **env)
{
	char	*arg_id;
	char	*arg_raw;
	t_env	*tmp;

	arg_id = get_env_id(value);
	if (!arg_id || !bi_check_id(arg_id))
		return (free(arg_id), bi_err_export(value));
	arg_raw = ft_strdup(value);
	if (!arg_raw)
		return (free(arg_id), 1);
	tmp = get_env(arg_id, *env);
	if (!tmp)
	{
		tmp = bi_new_var(arg_id, arg_raw);
		if (!tmp)
			return (1);
		env_add_back(env, tmp);
	}
	else
	{
		free(arg_id);
		bi_update_var(tmp, arg_raw);
	}
	return (0);
}



/** EXPORT */
int	ft_export(t_shell *content, t_arg *args)
{
	return (SUCCESS);
}

/** UNSET */
int	ft_unset(t_shell *content, t_arg *args)
{
	return (SUCCESS);
}
