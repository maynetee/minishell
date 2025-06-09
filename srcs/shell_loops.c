/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:40:24 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/30 18:25:21 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_readline_error(char *line, t_data *data)
{
	if (!line)
	{
		if (data->interactive_mode)
			ft_putendl_fd("exit", 2);
		clean_exit(data, data->last_exit_status);
	}
}

static int	handle_sigint(char *line, t_data *data)
{
	if (g_signal_received == SIGINT)
	{
		data->last_exit_status = 130;
		if (!*line)
		{
			free(line);
			data->current_line = NULL;
			return (1);
		}
		g_signal_received = 0;
	}
	return (0);
}

void	interactive_loop(t_data *data)
{
	char	*line;

	while (1)
	{
		setup_main_signal_handlers();
		g_signal_received = 0;
		line = readline("minishell> ");
		handle_readline_error(line, data);
		if (handle_sigint(line, data))
		{
			line = NULL;
			continue ;
		}
		if (*line)
			run_line(data, line);
		else
			data->current_line = line;
		if (data->current_line)
			free(data->current_line);
		data->current_line = NULL;
	}
}

void	non_interactive_loop(t_data *data)
{
	char	*line;
	size_t	len;

	line = get_next_line(0);
	while (line != NULL)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (*line)
		{
			run_line(data, line);
		}
		else
		{
			data->current_line = line;
		}
		free(data->current_line);
		data->current_line = NULL;
		line = get_next_line(0);
	}
}
