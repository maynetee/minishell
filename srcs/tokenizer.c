/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:30:54 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/30 18:26:41 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	try_tokenize_double_op(char *line, int i, char **buf, t_list **lst)
{
	char	c1;
	char	c2;

	c1 = line[i];
	c2 = '\0';
	if (line[i + 1])
		c2 = line[i + 1];
	if ((c1 == '>' && c2 == '>') || (c1 == '<' && c2 == '<'))
	{
		finalize_token(buf, lst);
		append_char_to_buffer(buf, c1);
		append_char_to_buffer(buf, c2);
		finalize_token(buf, lst);
		return (2);
	}
	return (0);
}

static int	process_token(t_tok_ctx *ctx, t_state *state, char *line, int *i)
{
	int		skip;
	char	next_char;

	if (line[*i])
		next_char = line[*i + 1];
	else
		next_char = '\0';
	skip = try_tokenize_double_op(line, *i, &ctx->buffer, &ctx->list);
	if (skip)
		return (skip);
	skip = dispatch_to_state_handler(line[*i], next_char, ctx, state);
	return (skip + 1);
}

t_list	*tokenize(char *line)
{
	t_tok_ctx	ctx;
	t_state		state;
	int			i;

	ft_bzero(&ctx, sizeof(ctx));
	state = GENERAL;
	i = 0;
	while (line[i])
		i += process_token(&ctx, &state, line, &i);
	if (!finalize_and_check(state, &ctx.buffer, &ctx.list))
		return (NULL);
	return (ctx.list);
}

static int	fill_array_and_clear(t_list *list, char **array)
{
	int		i;
	t_list	*cur;

	i = 0;
	cur = list;
	while (cur)
	{
		array[i] = ft_strdup(cur->content);
		if (!array[i])
		{
			free_tokens(array);
			ft_lstclear(&list, free);
			return (0);
		}
		cur = cur->next;
		i++;
	}
	array[i] = NULL;
	return (1);
}

char	**convert_token_list_to_array(t_list *list)
{
	int		size;
	char	**array;

	size = ft_lstsize(list);
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
	{
		ft_lstclear(&list, free);
		return (NULL);
	}
	if (!fill_array_and_clear(list, array))
		return (NULL);
	ft_lstclear(&list, free);
	return (array);
}
