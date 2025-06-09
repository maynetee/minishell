/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 23:22:12 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 16:54:25 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**expand_argv(t_data *data, char **raw_argv)
{
	int		i;
	int		count;
	char	**expanded_argv;
	char	*temp_expanded;

	count = 0;
	while (raw_argv && raw_argv[count])
		count++;
	expanded_argv = ft_calloc(count + 1, sizeof(char *));
	if (!expanded_argv)
		return (NULL);
	i = 0;
	while (i < count)
	{
		temp_expanded = perform_expansion(raw_argv[i], data);
		if (!temp_expanded)
		{
			free_tokens(expanded_argv);
			return (NULL);
		}
		expanded_argv[i] = temp_expanded;
		i++;
	}
	expanded_argv[i] = NULL;
	return (expanded_argv);
}

int	handle_simple_builtin(t_data *data, t_command *cmd)
{
	char	**expanded_argv;

	if (!cmd->infile && !cmd->outfile && cmd->heredoc_fd == -1 && cmd->argv
		&& cmd->argv[0] && is_builtin(cmd->argv[0]))
	{
		expanded_argv = expand_argv(data, cmd->argv);
		if (!expanded_argv)
		{
			data->last_exit_status = 1;
			close_redirection_fds(cmd);
			return (1);
		}
		dispatch_builtin(data, expanded_argv);
		free_tokens(expanded_argv);
		close_redirection_fds(cmd);
		return (1);
	}
	return (0);
}
