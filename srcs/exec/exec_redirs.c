#include "minishell.h"

void	redirs_type(t_filename *file)
{
    int fd = -1;
    int target_fd = STDOUT_FILENO;
    int flags = O_WRONLY | O_CREAT | O_TRUNC;

    if (file->type == INFILE || file->type == NON_HEREDOC)
    {
        target_fd = STDIN_FILENO;
        flags = O_RDONLY;
    }
    else if (file->type == APPEND)
        flags = O_WRONLY | O_CREAT | O_APPEND;
    fd = open(file->path, flags, 0644);
    if (fd == -1)
    {
        err_open(errno, file->path);
        return;
    }
    if (dup2(fd, target_fd) == -1)
    {
        err_open(errno, file->path);
        close(fd);
        return;
    }
    close(fd);
}

// goes through redirs list
// fd_in and fd_out = -1 if open failed so no need to close
int	exec_redirs(t_exec *exec)
{
	t_filename	*redirs;

	redirs = exec->redirs;
	while (redirs)
	{
		redirs_type(redirs);
		if (exec->fd_in == -1 || exec->fd_out == -1)
			return (1);
		redirs = redirs->next;
	}
	return (0);
}

// goes through exec list
int	err_redirs(t_exec *exec)
{
	while (exec)
	{
		if (exec_redirs(exec) == 1)
			return (1);
		exec = exec->next;
	}
	return (0);
}
