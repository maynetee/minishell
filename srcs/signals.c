/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:40:22 by mteichma          #+#    #+#             */
/*   Updated: 2025/05/01 00:26:23 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_main_handler(int signo)
{
	(void)signo;
	g_signal_received = SIGINT;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static void	sigint_heredoc_handler(int signo)
{
	(void)signo;
	g_signal_received = SIGINT;
	rl_done = 1;
	close(0);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	write(1, "\n", 1);
}

void	setup_main_signal_handlers(void)
{
	struct sigaction	sa_quit;
	struct sigaction	sa_int;
	struct sigaction	sa_tstp;
	struct termios		term;

	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
	ft_memset(&sa_tstp, 0, sizeof(sa_tstp));
	sa_tstp.sa_handler = SIG_IGN;
	sigemptyset(&sa_tstp.sa_mask);
	sa_tstp.sa_flags = 0;
	sigaction(SIGTSTP, &sa_tstp, NULL);
	ft_memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = sigint_main_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	if (!tcgetattr(0, &term))
	{
		term.c_lflag |= ECHOCTL;
		tcsetattr(0, TCSANOW, &term);
	}
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa_quit;
	struct sigaction	sa_int;
	struct sigaction	sa_tstp;

	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
	ft_memset(&sa_tstp, 0, sizeof(sa_tstp));
	sa_tstp.sa_handler = SIG_IGN;
	sigemptyset(&sa_tstp.sa_mask);
	sa_tstp.sa_flags = 0;
	sigaction(SIGTSTP, &sa_tstp, NULL);
	ft_memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = sigint_heredoc_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
}

void	setup_child_signal_handlers(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
