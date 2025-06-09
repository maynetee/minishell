/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:22:16 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 01:46:54 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_infile(const char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd < 0)
		handle_exec_error(infile, infile);
	if (dup2(fd, 0) < 0)
	{
		ft_putstr_fd("minishell: dup2: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		close(fd);
		exit(1);
	}
	close(fd);
}

static void	handle_outfile(const char *outfile, int append_mode)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(outfile, flags, 0644);
	if (fd < 0)
		handle_exec_error(outfile, outfile);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		ft_putstr_fd("minishell: dup2: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		close(fd);
		exit(1);
	}
	close(fd);
}

void	apply_redirections(t_command *cmd)
{
	if (cmd->heredoc_fd != -1)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
		{
			ft_putstr_fd("minishell: dup2: ", 2);
			ft_putendl_fd(strerror(errno), 2);
			exit(1);
		}
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->infile)
		handle_infile(cmd->infile);
	if (cmd->outfile)
		handle_outfile(cmd->outfile, cmd->append_mode);
}

void	close_redirection_fds(t_command *cmd)
{
	if (cmd && cmd->heredoc_fd != -1)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
}
