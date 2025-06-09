/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 03:58:17 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/30 18:28:05 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_heredoc_list(t_command *cmd)
{
	t_heredoc_spec	*current_hdoc;
	t_heredoc_spec	*tmp;

	current_hdoc = cmd->heredocs;
	while (current_hdoc)
	{
		tmp = current_hdoc->next;
		free(current_hdoc->delimiter);
		free(current_hdoc);
		current_hdoc = tmp;
	}
	cmd->heredocs = NULL;
	if (cmd->heredoc_fd != -1)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
}

void	handle_parser_input(t_command *cmd, t_list **tok)
{
	t_list	*next;
	char	*filename;

	next = (*tok)->next;
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
	free(cmd->infile);
	cmd->infile = filename;
	*tok = next->next;
}

static t_heredoc_spec	*create_heredoc_spec(char *raw_delim)
{
	t_heredoc_spec	*spec;
	int				quoted;

	spec = malloc(sizeof(*spec));
	if (!spec)
		return (NULL);
	spec->delimiter = remove_quotes(raw_delim);
	if (!spec->delimiter)
	{
		free(spec);
		return (NULL);
	}
	quoted = ft_strpbrk(raw_delim, "'\"") != NULL;
	spec->expand = !quoted;
	spec->next = NULL;
	return (spec);
}

static void	append_heredoc_spec(t_command *cmd, t_heredoc_spec *spec)
{
	t_heredoc_spec	*tail;

	if (!cmd->heredocs)
		cmd->heredocs = spec;
	else
	{
		tail = cmd->heredocs;
		while (tail->next)
			tail = tail->next;
		tail->next = spec;
	}
}

void	handle_parser_heredoc(t_command *cmd, t_list **tok)
{
	t_list			*next;
	t_heredoc_spec	*spec;

	free(cmd->infile);
	cmd->infile = NULL;
	next = (*tok)->next;
	if (!next)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			2);
		*tok = NULL;
		return ;
	}
	spec = create_heredoc_spec(next->content);
	if (spec)
		append_heredoc_spec(cmd, spec);
	*tok = next->next;
}
