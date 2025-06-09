/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:22:14 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 17:37:15 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_heredoc_line(t_heredoc_params *params, char *line)
{
	char	*out;

	out = line;
	if (params->expand)
	{
		out = perform_expansion(line, params->data);
		if (!out)
			out = line;
	}
	return (out);
}

static int	write_heredoc_output(int fd, char *processed_line, char *orig_line)
{
	if (write(fd, processed_line, ft_strlen(processed_line)) < 0
		|| write(fd, "\n", 1) < 0)
	{
		perror("minishell: write heredoc");
		if (processed_line != orig_line)
			free(processed_line);
		return (free(orig_line), -1);
	}
	if (processed_line != orig_line)
		free(processed_line);
	return (free(orig_line), 1);
}

int	handle_heredoc_line(t_heredoc_params *params, char *line)
{
	char	*out;

	if (!ft_strcmp(line, params->delimiter))
		return (free(line), 0);
	out = expand_heredoc_line(params, line);
	return (write_heredoc_output(params->write_fd, out, line));
}
