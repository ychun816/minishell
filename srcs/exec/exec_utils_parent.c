#include "minishell.h"

int	open_pipes(int pipes_nb, int (*fd)[2])
{
	int	i;
	int	j;

	i = 0;
	while (i < pipes_nb)
	{
		if (pipe(fd[i]) == -1)
		{
			j = 0;
			while (j < i)
			{
				exe_close(&fd[j][0]);
				exe_close(&fd[j][1]);
				j++;
			}
			return (-1);
		}
		i++;
	}
	return (0);
}

// only for parent process
// mode 0 : saves a copy of STDIN and STDOUT
// mode 1 : restores STDIN and STDOUT + free the copy
void	set_std(t_shell *ctx, int mode)
{
	if (!mode)
	{
		ctx->default_in = dup(STDIN_FILENO);
		ctx->default_out = dup(STDOUT_FILENO);
	}
	else
	{
		dup2(ctx->default_in, STDIN_FILENO);
		exe_close(&(ctx->default_in));
		dup2(ctx->default_out, STDOUT_FILENO);
		exe_close(&(ctx->default_out));
	}
}

// only for parent process
void	close_all(int pipe_nb, int (*fd)[2])
{
	int	i;

	i = 0;
	while (i < pipe_nb)
	{
		exe_close(&fd[i][0]);
		exe_close(&fd[i][1]);
		i++;
	}
}

