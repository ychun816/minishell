/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_func_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 09:09:45 by tsuchen           #+#    #+#             */
/*   Updated: 2024/12/08 15:40:27 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_echo(t_arg *args)
{
	int	n_flag;

	n_flag = 0;
	while (args && !bi_is_nflag(args->value))
	{
		n_flag = 1;
		args = args->next;
	}
	while (args)
	{
		printf("%s", args->value);
		if (args->next)
			printf("%s", " ");
		args = args->next;
	}
	if (!n_flag)
		printf("%s", "\n");
	return (0);
}

int	bi_cd(t_shell *ctx, t_arg *args)
{
	int		siz;
	char	*cwd;
	t_env	*home;

	siz = ft_arg_lstsize(args);
	if (siz > 1)
		return (ft_putstr_fd("minishell: cd: too many arguments\n",
				STDERR_FILENO), 1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell: cd: error retrieving current directory");
	home = get_env("HOME", ctx->env);
	if ((!siz || !ft_strcmp(args->value, "--")) && home && home->value)
		chdir(home->value);
	else if ((!siz || !ft_strcmp(args->value, "--")) && (!home || !home->value))
		return (bi_err_cd(errno, "HOME"), free(cwd), 1);
	else if (chdir(args->value) < 0)
	{
		bi_err_cd(errno, args->value);
		return (free(cwd), 1);
	}
	if (bi_update_pwd(ctx, cwd))
		return (free(cwd), 1);
	return (free(cwd), 0);
}

int	bi_pwd(t_arg *args)
{
	char	*cwd;

	if (args && *(args->value) == '-' && (ft_strcmp(args->value, "-L")
			&& ft_strcmp(args->value, "-P")))
	{
		bi_err_pwd(args->value);
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

int	bi_exit(t_shell *ctx, t_arg *args)
{
	int	exit_code;

	if (args && args->next && !bi_check_exitcode(args->value))
		return (ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO), 1);
	exit_code = 0;
	if (args && !bi_check_exitcode(args->value))
		exit_code = ft_atoi(args->value);
	else if (args && bi_check_exitcode(args->value))
	{
		bi_err_exit(args->value);
		exit_code = 2;
	}
	ft_close(ctx);
	free_all_shell(ctx);
	exit(exit_code);
}

int	bi_env(t_shell *ctx, t_arg *args)
{
	t_env	*tmp;

	tmp = ctx->env;
	if (args)
	{
		bi_err_env(args->value);
		return (127);
	}
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->id, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
