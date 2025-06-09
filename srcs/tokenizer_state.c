/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_state.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:18:44 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/30 18:23:46 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_separator(char c, char **buffer, t_list **list)
{
	finalize_token(buffer, list);
	append_char_to_buffer(buffer, c);
	finalize_token(buffer, list);
}

int	handle_general_state(char c, char **buffer, t_list **list, t_state *state)
{
	if (c == '\'')
	{
		append_char_to_buffer(buffer, c);
		*state = IN_SQ;
	}
	else if (c == '"')
	{
		append_char_to_buffer(buffer, c);
		*state = IN_DQ;
	}
	else if (ft_isspace(c))
		finalize_token(buffer, list);
	else if (c == '|' || c == '<' || c == '>')
		handle_separator(c, buffer, list);
	else
		append_char_to_buffer(buffer, c);
	return (0);
}

int	handle_sq_state(char c, char **buffer, t_state *state)
{
	append_char_to_buffer(buffer, c);
	if (c == '\'')
		*state = GENERAL;
	return (1);
}

int	handle_dq_state(char c, char **buffer, t_state *state, char next_char)
{
	(void)next_char;
	append_char_to_buffer(buffer, c);
	if (c == '"')
		*state = GENERAL;
	return (0);
}
