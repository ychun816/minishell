/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:02:35 by okoca             #+#    #+#             */
/*   Updated: 2024/12/23 14:30:01 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../includes/minishell.h"

# define P_NAME "minishell"

typedef struct s_shell		t_shell;
typedef struct s_env		t_env;
typedef enum e_token_type	t_token_type;
typedef struct s_token		t_token;
typedef struct s_arg		t_arg;
typedef struct s_filename	t_filename;

/* Builtins handler*/
int		check_is_builtin(char *input_cmd);
int		exec_builtin(t_shell *content, char *input_cmd, t_arg *args);
int		add_envvar(char *env_line, t_env **env_head);
int		check_envid_valid(char *env_id);
void	update_envvar(t_env *update_env, char *env_line);

/** ft_env / ft_pwd */
int		ft_env(t_shell *content, t_arg *args);
int		ft_pwd(t_arg *args);

/* ft_echo*/
int		ft_echo(t_arg *args);
int		check_echo_has_nargs(char *flag);

/* ft_exit*/
int		ft_exit(t_shell *content, t_arg *args);
int		check_exitcode(char	*input_line);

/* ft_cd */
int		ft_cd(t_shell *content, t_arg *args);
int		update_pwd(t_shell *content, char *oldpwd_value);
int		ft_arg_lstsize(t_arg *args);

/* ft_export */
int		ft_export(t_shell *content, t_arg *args);
int		export_print_ordered_env(t_env *env);
char	**sort_env_arrs(char **env_arrs);

/* ft_unset */
int		ft_unset(t_shell *content, t_arg *args);
int		delete_env_var(char *env_value, t_env **env);
void	env_delete_1node(t_env *env);

/** builtins error */
void	error_env(char *input_line);
void	error_pwd(char	*option);
void	error_exit(char *input_line);
void	error_cd(int err_n, char *pathname);
void	error_export(char *env_line);
void	error_unset(char *input_line);

///////////////////////////////////////////////////////////
///////////////////////   REF    //////////////////////////
///////////////////////////////////////////////////////////
/* Built-in handler */
// int		bi_do_builtin(t_shell *ctx, char *cmd, t_arg *args);
// int		bi_is_builtin(char *cmd);

// /* Built-in Functions */
// int		bi_echo(t_arg *args);
// int		bi_cd(t_shell *ctx, t_arg *args);
// int		bi_pwd(t_arg *args);
// int		bi_exit(t_shell *ctx, t_arg *args);
// int		bi_env(t_shell *ctx, t_arg *args);
// int		bi_export(t_shell *ctx, t_arg *args);
// int		bi_unset(t_shell *ctx, t_arg *args);

// /* helper functions builtins */
// int		bi_print_export(t_env *env);
// int		bi_add_var(char *value, t_env **env);
// int		bi_del_var(char *value, t_env **env);
// int		bi_check_id(char *id);
// int		bi_check_exitcode(char *value);
// int		bi_update_pwd(t_shell *ctx, char *value);
// int		bi_is_nflag(char *flag);

// /* error utils */
// void	bi_err_cd(int err_no, char *file);
// void	bi_err_pwd(char *option);
// int		bi_err_export(char *var);
// void	bi_err_exit(char *val);
// void	bi_err_env(char *file);

// /* ADDED HELPER FUNC*/ //CHECK LATER
// void	ft_advanced_sort_string_tab(char **tab,
// 	int (*cmp)(const char *, const char *));
// int	ft_arg_lstsize(t_arg *args);

#endif
