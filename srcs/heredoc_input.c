/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 04:10:19 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 17:15:00 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_eof_or_interrupt(t_heredoc_params *params, char *line)
{
	if (g_signal_received == SIGINT)
	{
		params->data->last_exit_status = 130;
		return (0);
	}
	if (!line)
	{
		if (ft_strlen(params->delimiter) != 0)
			ft_putendl_fd("minishell: warning: heredoc delimited by EOF", 2);
		return (1);
	}
	return (-1);
}

static int	read_heredoc_loop(t_heredoc_params *params)
{
	char	*line;
	int		ret;
	int		eof_ret;

	ret = 1;
	while (ret == 1)
	{
		g_signal_received = 0;
		line = readline("> ");
		eof_ret = handle_eof_or_interrupt(params, line);
		if (eof_ret != -1)
		{
			free(line);
			return (eof_ret);
		}
		ret = handle_heredoc_line(params, line);
		if (ret == -1)
			return (0);
	}
	return (ret == 0);
}

static int	run_heredoc_read_loop(t_data *data, const char *delim, int expand,
		int write_fd)
{
	t_heredoc_params	params;
	int					success;

	params.data = data;
	params.delimiter = delim;
	params.expand = expand;
	params.write_fd = write_fd;
	success = read_heredoc_loop(&params);
	return (success);
}

int	read_heredoc_input(t_data *data, const char *delim, int expand,
		int write_fd)
{
	int	stdin_backup;
	int	success;

	stdin_backup = -1;
	if (!setup_heredoc_io(&stdin_backup))
		return (0);
	setup_heredoc_signals();
	g_signal_received = 0;
	success = run_heredoc_read_loop(data, delim, expand, write_fd);
	cleanup_after_heredoc(stdin_backup);
	return (success);
}
