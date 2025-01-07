/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 17:46:01 by varodrig          #+#    #+#             */
/*   Updated: 2025/01/07 18:26:20 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" //TODO

/*
typedef struct s_signal
{
	int				end_heredoc;
	int				signal_code;

}					t_signal;
*/
//SIGINT = 2 int <signal.h>
//"if" so we initialise only 1 time
//SIG_OFFSET so we know its 
static void	init_sigint(int status)
{
	(void)status;
	if (g_signal.signal_code != 1)
	{
		g_signal.signal_code = SIGINT + SIG_OFFSET;
		write(STDIN_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
int	sig_event(void)
{
	return (EXIT_SUCCESS);
}

//called in the begining of the while(1) in main
//SIGQUIT = Ctrl+\ does nothing 
//SIGSTP = Ctrl+Z does nothing 
void	init_signals(void)
{
	rl_event_hook = sig_event;
	signal(SIGINT, init_sigint);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

//read by the parent so it doesnt exit the program
//when CTRL+C is pressed
void	sigint_exec(int status)
{
	(void)status;
	g_signal.signal_code = SIGINT + SIG_OFFSET ;
	write(STDERR_FILENO, "\n", 1);
}

void	sig_heredoc(int status)
{
	(void)status;
	rl_replace_line("", 0);
	rl_redisplay();
	rl_done = 1;
	g_signal.end_heredoc = 1;
	g_signal.signal_code = SIG_OFFSET + SIGINT;
}
