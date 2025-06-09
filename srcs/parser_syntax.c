/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:21:33 by mteichma          #+#    #+#             */
/*   Updated: 2025/06/04 18:50:12 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	syntax_error(t_list **args, t_sequence *seq_head)
{
	(void)seq_head;
	ft_lstclear(args, free);
	return (0);
}

static int	is_operator(char *val)
{
	if (!val)
		return (0);
	return (ft_strcmp(val, "|") == 0 || ft_strcmp(val, "<") == 0
		|| ft_strcmp(val, ">") == 0 || ft_strcmp(val, ">>") == 0
		|| ft_strcmp(val, "<<") == 0);
}

static int	check_redirect_syntax(t_list *tok)
{
	char	*val;
	char	*next_val;

	val = tok->content;
	next_val = NULL;
	if (tok->next)
		next_val = tok->next->content;
	if (is_operator(val) && ft_strcmp(val, "|") != 0)
	{
		if (!tok->next)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putendl_fd("newline'", 2);
			return (0);
		}
		if (is_operator(next_val))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(next_val, 2);
			ft_putendl_fd("'", 2);
			return (0);
		}
	}
	return (1);
}

static int	check_separator_syntax(t_list *tok)
{
	char	*val;
	char	*next_val;

	val = tok->content;
	next_val = NULL;
	if (tok->next)
		next_val = tok->next->content;
	if (ft_strcmp(val, "|") == 0)
	{
		if (!tok->next || !ft_strcmp(next_val, "|"))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(val, 2);
			ft_putendl_fd("'", 2);
			return (0);
		}
	}
	return (1);
}

int	check_syntax(t_list *tok, t_list **args, t_sequence *seq_head)
{
	if (!tok)
		return (1);
	if (!check_redirect_syntax(tok))
		return (syntax_error(args, seq_head));
	if (!check_separator_syntax(tok))
		return (syntax_error(args, seq_head));
	(void)args;
	(void)seq_head;
	return (1);
}
