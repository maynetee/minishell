/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 00:15:15 by lupetit           #+#    #+#             */
/*   Updated: 2025/04/30 18:24:40 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_sequence	*create_new_sequence(t_command *pipeline)
{
	t_sequence	*seq;

	seq = malloc(sizeof(*seq));
	if (!seq)
		return (NULL);
	seq->pipeline_head = pipeline;
	seq->next = NULL;
	return (seq);
}

static int	finalize_command(t_parser_state *state)
{
	if (!state->current_cmd)
		return (1);
	state->current_cmd->argv = convert_token_list_to_array(state->args_list);
	state->args_list = NULL;
	if (!state->current_cmd->argv && errno == ENOMEM)
		return (0);
	return (1);
}

static int	handle_separator_token(t_parser_state *state, char *token_content)
{
	if (!finalize_command(state))
		return (0);
	if (!ft_strcmp(token_content, "|"))
	{
		handle_pipe_token(&state->current_cmd, &state->args_list);
		if (!state->current_cmd)
			return (0);
	}
	state->tok_ptr = state->tok_ptr->next;
	return (1);
}

static int	parse_token_loop(t_parser_state *s)
{
	char	*tok;

	if (!check_first_token(s->tok_ptr))
		return (0);
	while (s->tok_ptr)
	{
		if (!check_syntax(s->tok_ptr, &s->args_list, s->seq_head))
			return (0);
		if (!s->current_cmd && !start_new_command(s))
			return (0);
		tok = s->tok_ptr->content;
		if (!ft_strcmp(tok, "|"))
		{
			if (!handle_separator_token(s, tok))
				return (0);
		}
		else
		{
			if (!handle_regular_token(s, tok))
				return (0);
		}
	}
	return (1);
}

t_sequence	*parse_input_line(t_data *data, t_list *tokens)
{
	t_parser_state	state;

	if (!tokens)
		return (NULL);
	ft_bzero(&state, sizeof(state));
	state.data = data;
	state.tok_ptr = tokens;
	state.seq_head = create_new_sequence(NULL);
	if (!state.seq_head)
		return (NULL);
	state.current_seq = state.seq_head;
	if (!parse_token_loop(&state))
	{
		free_sequence_list(state.seq_head);
		data->last_exit_status = 2;
		return (NULL);
	}
	if (!finalize_command(&state))
	{
		free_sequence_list(state.seq_head);
		return (NULL);
	}
	return (state.seq_head);
}
