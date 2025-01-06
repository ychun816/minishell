/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 17:46:01 by varodrig          #+#    #+#             */
/*   Updated: 2025/01/06 17:46:05 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" //TODO

int	sig_event(void)
{
	return (EXIT_SUCCESS);
}

void	sig_init_signals(void)
{
	rl_event_hook = sig_event;
	signal(SIGINT, sig_int_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	sig_int_handler(int status)
{
	(void)status;
	if (g_signal.signal_code != 1)
	{
		g_signal.signal_code = SIGNAL_OFFSET + SIGINT;
		write(STDIN_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sig_exec(int status)
{
	(void)status;
	g_signal.signal_code = SIGNAL_OFFSET + SIGINT;
	write(STDERR_FILENO, "\n", 1);
}

void	sig_heredoc(int status)
{
	(void)status;
	rl_replace_line("", 0);
	rl_redisplay();
	rl_done = 1;
	g_signal.end_heredoc = 1;
	g_signal.signal_code = SIGNAL_OFFSET + SIGINT;
}
