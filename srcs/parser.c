/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:14:01 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 01:53:49 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredocs = NULL;
	cmd->heredoc_fd = -1;
	cmd->append_mode = 0;
	cmd->next = NULL;
	return (cmd);
}

static void	free_command(t_command *cmd)
{
	int				i;
	t_heredoc_spec	*tmp;

	if (!cmd)
		return ;
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
			free(cmd->argv[i++]);
		free(cmd->argv);
	}
	free(cmd->infile);
	free(cmd->outfile);
	while (cmd->heredocs)
	{
		tmp = cmd->heredocs->next;
		free(cmd->heredocs->delimiter);
		free(cmd->heredocs);
		cmd->heredocs = tmp;
	}
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	free(cmd);
}

static void	free_command_list(t_command *list)
{
	t_command	*next;

	while (list)
	{
		next = list->next;
		free_command(list);
		list = next;
	}
}

void	free_sequence_list(t_sequence *seq_list)
{
	t_sequence	*next_seq;

	while (seq_list)
	{
		next_seq = seq_list->next;
		free_command_list(seq_list->pipeline_head);
		free(seq_list);
		seq_list = next_seq;
	}
}
