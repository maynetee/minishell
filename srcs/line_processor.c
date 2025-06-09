/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_processor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lupetit <lupetit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:43:30 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/27 19:34:32 by lupetit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_line(t_data *data, char *line)
{
	t_list		*tokens;
	t_sequence	*seq_list;

	data->current_line = line;
	if (data->interactive_mode && *line)
		add_history(line);
	tokens = tokenize(line);
	if (!tokens)
	{
		data->last_exit_status = 2;
		return ;
	}
	seq_list = parse_input_line(data, tokens);
	ft_lstclear(&tokens, free);
	if (!seq_list)
	{
		return ;
	}
	execute_sequences(data, seq_list);
	free_sequence_list(seq_list);
}
