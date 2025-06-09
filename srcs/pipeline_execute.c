/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 00:18:15 by lupetit           #+#    #+#             */
/*   Updated: 2025/05/01 00:29:15 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	collect_pids(t_data *data, t_command *cmd_list, pid_t *pids)
{
	int		i;
	int		in_fd;
	pid_t	pid;

	i = 0;
	in_fd = -1;
	while (cmd_list)
	{
		pid = spawn_cmd(data, cmd_list, &in_fd);
		if (pid < 0)
		{
			data->last_exit_status = 1;
			if (in_fd != -1)
				close(in_fd);
			break ;
		}
		pids[i++] = pid;
		cmd_list = cmd_list->next;
	}
	if (in_fd != -1)
		close(in_fd);
	return (i);
}

static void	wait_for_children(t_data *data, pid_t *pids, int count)
{
	int	i;
	int	status;

	ignore_signals();
	i = 0;
	while (i < count - 1)
	{
		if (pids[i] > 0)
			waitpid(pids[i], NULL, 0);
		i++;
	}
	status = 0;
	if (count > 0 && pids[count - 1] > 0)
		waitpid(pids[count - 1], &status, 0);
	update_exit_status(data, status);
	setup_main_signal_handlers();
}

void	execute_pipeline(t_data *data, t_command *cmd_list)
{
	int			cmd_count;
	pid_t		*pids;
	int			spawned;
	t_command	*tmp;

	cmd_count = 0;
	tmp = cmd_list;
	while (tmp)
	{
		cmd_count++;
		tmp = tmp->next;
	}
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		data->last_exit_status = 1;
		return ;
	}
	spawned = collect_pids(data, cmd_list, pids);
	wait_for_children(data, pids, spawned);
	free(pids);
}
