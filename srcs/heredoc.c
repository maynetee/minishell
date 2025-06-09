/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:51:23 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/28 23:07:31 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_heredocs(t_heredoc_spec *list)
{
	int	n;

	n = 0;
	while (list)
	{
		n++;
		list = list->next;
	}
	return (n);
}

static int	process_one_doc(t_data *d, t_heredoc_spec *s,
							t_command *cmd, int keep_fd)
{
	int	pfd[2];
	int	ok;

	if (pipe(pfd) < 0)
		return (perror("minishell: pipe heredoc"), 0);
	ok = read_heredoc_input(d, s->delimiter, s->expand, pfd[1]);
	close(pfd[1]);
	if (!ok)
	{
		close(pfd[0]);
		return (0);
	}
	if (keep_fd)
		cmd->heredoc_fd = pfd[0];
	else
		close(pfd[0]);
	return (1);
}

static void	close_heredoc_fds(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		if (current->heredoc_fd != -1)
		{
			close(current->heredoc_fd);
			current->heredoc_fd = -1;
		}
		current = current->next;
	}
}

int	handle_heredocs(t_data *data, t_command *cmd_list)
{
	t_command		*current_cmd;
	t_heredoc_spec	*current_spec;
	int				n;
	int				is_last;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		current_spec = current_cmd->heredocs;
		n = count_heredocs(current_spec);
		while (current_spec)
		{
			n--;
			is_last = (n == 0);
			if (!process_one_doc(data, current_spec, current_cmd, is_last))
			{
				close_heredoc_fds(cmd_list);
				return (0);
			}
			current_spec = current_spec->next;
		}
		current_cmd = current_cmd->next;
	}
	return (1);
}
