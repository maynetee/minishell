/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 04:12:17 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 18:26:44 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_signaled_exit(t_data *data, int status)
{
	int	term_sig;

	term_sig = WTERMSIG(status);
	data->last_exit_status = 128 + term_sig;
	if (term_sig == SIGQUIT)
	{
		if ((status & 0x80) != 0)
			ft_putendl_fd("Quit (core dumped)", 2);
		else
			ft_putendl_fd("Quit", 2);
	}
	else if (term_sig == SIGINT)
	{
		write(2, "\n", 1);
	}
}

void	update_exit_status(t_data *data, int status)
{
	if (WIFSIGNALED(status))
		handle_signaled_exit(data, status);
	else if (WIFEXITED(status))
		data->last_exit_status = WEXITSTATUS(status);
	else
		data->last_exit_status = 1;
}
