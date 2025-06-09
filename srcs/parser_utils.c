/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:40:48 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 17:47:41 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redir(char *s, char *op)
{
	return (ft_strcmp(s, op) == 0);
}

static void	handle_argument_token(t_data *data, char *val, t_list **args)
{
	char	*raw_val;
	t_list	*new_node;

	(void)data;
	raw_val = ft_strdup(val);
	if (!raw_val)
		return ;
	new_node = ft_lstnew(raw_val);
	if (new_node)
		ft_lstadd_back(args, new_node);
	else
		free(raw_val);
}

void	process_parser_token(t_data *data, t_command *cmd, t_list **args,
		t_list **tok_ptr)
{
	char	*val;
	t_list	*current_tok;

	current_tok = *tok_ptr;
	if (!current_tok || !current_tok->content)
		return ;
	val = current_tok->content;
	if (is_redir(val, ">>") || is_redir(val, ">"))
		handle_parser_output(cmd, tok_ptr);
	else if (is_redir(val, "<"))
		handle_parser_input(cmd, tok_ptr);
	else if (is_redir(val, "<<"))
		handle_parser_heredoc(cmd, tok_ptr);
	else
	{
		handle_argument_token(data, val, args);
		*tok_ptr = current_tok->next;
	}
}
