#ifndef EXEC_H
# define EXEC_H

# include "../includes/minishell.h"

typedef struct s_shell	t_shell;
typedef struct s_env	t_env;
typedef struct s_exec	t_exec;
typedef struct s_arg	t_arg;
typedef struct s_filename	t_filename;


void					exe_close(int *fd);
void					ft_close(t_shell *ctx);
int						open_pipes(int pipes_nb, int fd[pipes_nb][2]);
void					close_fds(int pipes_nb, int (*fd)[2], int current_cmd,
							bool is_final_close);
int						size_linked_list(t_arg *args);
void					create_args(t_exec *temp, int args_nb,
							char *args[args_nb]);
int						ft_char_count(char *str, char c);
void					ft_free_all(char **arr);
char					*find_path(char *cmd, t_env *env);
int						size_env(t_env *env);
char					**env_format(t_env *env);
int						ft_execution(t_shell *ctx, t_exec *temp);
void					child_process(t_shell *ctx, int (*fd)[2], int i,
							t_exec *temp);
void					exe_wait_all(t_shell *ctx);
int						exec(t_shell *ctx);
int						exec_parent(t_shell *ctx);
void					set_fds(t_shell *ctx, int mode);
void					unlink_all(t_shell *ctx);
int						err_redirs(t_exec *exec);
int						exec_redirs(t_exec *exec);
int						redirs_type(t_exec *exec, t_filename *file);

#endif