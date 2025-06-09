/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execute_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:28:21 by mteichma          #+#    #+#             */
/*   Updated: 2025/05/01 00:31:11 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_pipe(t_command *cmd, int fds[2])
{
	if (cmd->next)
	{
		if (pipe(fds) < 0)
		{
			perror("minishell: pipe");
			return (0);
		}
	}
	else
	{
		fds[0] = -1;
		fds[1] = -1;
	}
	return (1);
}

static pid_t	fork_cmd(int fds[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		if (fds[0] != -1)
			close(fds[0]);
		if (fds[1] != -1)
			close(fds[1]);
		perror("minishell: fork");
	}
	return (pid);
}

static void	child_process(t_data *data, t_command *cmd, int in_fd, int fds[2])
{
	if (in_fd != -1)
	{
		if (dup2(in_fd, 0) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
	}
	if (fds[1] != -1)
	{
		if (dup2(fds[1], 1) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		close(fds[0]);
		close(fds[1]);
	}
	if (in_fd != -1)
		close(in_fd);
	setup_child_signal_handlers();
	apply_redirections(cmd);
	execute_command_internal(data, cmd);
}

static void	cleanup_parent(int *in_fd_ptr, int fds[2])
{
	if (*in_fd_ptr != -1)
		close(*in_fd_ptr);
	if (fds[1] != -1)
	{
		close(fds[1]);
		*in_fd_ptr = fds[0];
	}
	else
		*in_fd_ptr = -1;
}

pid_t	spawn_cmd(t_data *data, t_command *cmd, int *in_fd_ptr)
{
	int		fds[2];
	pid_t	pid;

	if (!init_pipe(cmd, fds))
		return (-1);
	pid = fork_cmd(fds);
	if (pid < 0)
		return (-1);
	if (pid == 0)
		child_process(data, cmd, *in_fd_ptr, fds);
	cleanup_parent(in_fd_ptr, fds);
	return (pid);
}
