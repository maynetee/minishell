/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_output.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 04:05:17 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 17:38:30 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parser_output(t_command *cmd, t_list **tok)
{
	t_list	*next;
	char	*filename;
	char	*redir_op;

	next = (*tok)->next;
	redir_op = (*tok)->content;
	if (!next)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			2);
		*tok = NULL;
		return ;
	}
	filename = ft_strdup(next->content);
	if (!filename)
		return ;
	free(cmd->outfile);
	cmd->outfile = filename;
	if (!ft_strcmp(redir_op, ">>"))
		cmd->append_mode = 1;
	else
		cmd->append_mode = 0;
	*tok = next->next;
}
