/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_sequence.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:41:29 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 18:20:34 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_heredoc_specs(t_command *cmd)
{
	while (cmd)
	{
		clear_heredoc_list(cmd);
		cmd = cmd->next;
	}
}

void	execute_sequences(t_data *data, t_sequence *seq_list)
{
	t_sequence	*current_seq;
	int			heredoc_ok;
	t_command	*first_cmd;

	current_seq = seq_list;
	while (current_seq)
	{
		first_cmd = current_seq->pipeline_head;
		if (first_cmd)
		{
			heredoc_ok = handle_heredocs(data, first_cmd);
			if (!heredoc_ok)
			{
				data->last_exit_status = 1;
				current_seq = current_seq->next;
				continue ;
			}
			if (first_cmd->next == NULL)
				execute_command(data, first_cmd);
			else
				execute_pipeline(data, first_cmd);
			free_heredoc_specs(first_cmd);
		}
		current_seq = current_seq->next;
	}
}
