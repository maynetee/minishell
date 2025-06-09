/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:53:29 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/24 22:33:35 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	finalize_and_check(t_state state, char **buffer, t_list **list)
{
	finalize_token(buffer, list);
	if (state != GENERAL)
	{
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
		ft_lstclear(list, free);
		*list = NULL;
		return (0);
	}
	return (1);
}

void	append_char_to_buffer(char **str, char c)
{
	char	*temp;
	size_t	old_len;

	old_len = 0;
	if (*str)
		old_len = ft_strlen(*str);
	temp = ft_realloc(*str, old_len, old_len + 2);
	if (!temp)
	{
		free(*str);
		*str = NULL;
		return ;
	}
	*str = temp;
	(*str)[old_len] = c;
	(*str)[old_len + 1] = '\0';
}

void	finalize_token(char **buffer, t_list **list)
{
	t_list	*node;

	if (!buffer || !*buffer || !**buffer)
	{
		if (buffer && *buffer)
		{
			free(*buffer);
			*buffer = NULL;
		}
		return ;
	}
	node = ft_lstnew(*buffer);
	if (node)
		ft_lstadd_back(list, node);
	else
		free(*buffer);
	*buffer = NULL;
}

int	dispatch_to_state_handler(char current_char, char next_char, t_tok_ctx *ctx,
		t_state *state)
{
	int	skip_next;

	skip_next = 0;
	if (*state == GENERAL)
		handle_general_state(current_char, &ctx->buffer, &ctx->list, state);
	else if (*state == IN_SQ)
		handle_sq_state(current_char, &ctx->buffer, state);
	else
		skip_next = handle_dq_state(current_char, &ctx->buffer, state,
				next_char);
	return (skip_next);
}
