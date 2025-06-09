/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 23:20:51 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/30 22:58:53 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_token(t_command **current_cmd, t_list **args_list)
{
	*args_list = NULL;
	(*current_cmd)->next = create_new_command();
	*current_cmd = (*current_cmd)->next;
}

int	check_first_token(t_list *token)
{
	if (!token)
		return (1);
	if (!ft_strcmp(token->content, "|"))
	{
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `|'\n",
			2);
		return (0);
	}
	return (1);
}

int	start_new_command(t_parser_state *s)
{
	s->current_cmd = create_new_command();
	if (!s->current_cmd)
		return (0);
	if (!s->current_seq->pipeline_head)
		s->current_seq->pipeline_head = s->current_cmd;
	return (1);
}

int	handle_regular_token(t_parser_state *s, char *tok)
{
	(void)tok;
	process_parser_token(s->data, s->current_cmd, &s->args_list, &s->tok_ptr);
	return (1);
}
