/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:02:35 by okoca             #+#    #+#             */
/*   Updated: 2024/12/07 18:07:42 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../includes/minishell.h"

# define P_NAME "minishell"

/* Built-in handler */
int		bi_do_builtin(t_shell *ctx, char *cmd, t_arg *args);
int		bi_is_builtin(char *cmd);

/* Built-in Functions */
int		bi_echo(t_arg *args);
int		bi_cd(t_shell *ctx, t_arg *args);
int		bi_pwd(t_arg *args);
int		bi_exit(t_shell *ctx, t_arg *args);
int		bi_env(t_shell *ctx, t_arg *args);
int		bi_export(t_shell *ctx, t_arg *args);
int		bi_unset(t_shell *ctx, t_arg *args);

/* helper functions builtins */
int		bi_print_export(t_env *env);
int		bi_add_var(char *value, t_env **env);
int		bi_del_var(char *value, t_env **env);
int		bi_check_id(char *id);
int		bi_check_exitcode(char *value);
int		bi_update_pwd(t_shell *ctx, char *value);
int		bi_is_nflag(char *flag);

/* error utils */
void	bi_err_cd(int err_no, char *file);
void	bi_err_pwd(char *option);
int		bi_err_export(char *var);
void	bi_err_exit(char *val);
void	bi_err_env(char *file);


/* ADDED HELPER FUNC*/ //CHECK LATER
void	ft_advanced_sort_string_tab(char **tab,
	int (*cmp)(const char *, const char *));


#endif
