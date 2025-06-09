/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redir_check.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:49:39 by mteichma          #+#    #+#             */
/*   Updated: 2025/06/04 18:49:42 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_error(t_data *data, const char *path)
{
	ft_putstr_fd("minishell: ", 2);
	perror(path);
	data->last_exit_status = 1;
	return (0);
}

int	validate_redirections(t_data *data, t_command *cmd)
{
	int	fd;
	int	flags;

	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
			return (open_error(data, cmd->infile));
		close(fd);
	}
	if (!cmd->outfile)
		return (1);
	flags = O_WRONLY | O_CREAT;
	if (cmd->append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->outfile, flags, 0644);
	if (fd < 0)
		return (open_error(data, cmd->outfile));
	close(fd);
	return (1);
}
