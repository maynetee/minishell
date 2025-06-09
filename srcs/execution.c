/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 20:53:39 by mteichma          #+#    #+#             */
/*   Updated: 2025/06/04 18:50:28 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_expanded_argv_or_exit(t_data *data, t_command *cmd)
{
	char	**expanded_argv;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		exit(0);
	expanded_argv = expand_argv(data, cmd->argv);
	if (!expanded_argv)
		exit(1);
	if (!expanded_argv[0])
	{
		free_tokens(expanded_argv);
		exit(0);
	}
	setup_child_signal_handlers();
	return (expanded_argv);
}

static void	parent_wait(t_data *data, pid_t pid, t_command *cmd)
{
	int	status;

	ignore_signals();
	if (waitpid(pid, &status, 0) < 0)
	{
		if (errno != ECHILD && errno != EINTR)
			perror("minishell: waitpid");
		if (errno != EINTR)
			data->last_exit_status = 1;
	}
	else
		update_exit_status(data, status);
	close_redirection_fds(cmd);
	setup_main_signal_handlers();
	if (cmd && cmd->argv && cmd->argv[0] && ft_strcmp(cmd->argv[0],
			"exit") == 0)
		clean_exit(data, data->last_exit_status);
}

void	execute_command_internal(t_data *data, t_command *cmd)
{
	char	*path;
	char	*cmd_name;
	char	**expanded_argv;

	expanded_argv = get_expanded_argv_or_exit(data, cmd);
	cmd_name = expanded_argv[0];
	apply_redirections(cmd);
	if (execute_builtin(data, cmd))
		exit(data->last_exit_status);
	path = find_command_in_path(data, cmd_name);
	if (!path)
		handle_no_path(cmd_name);
	execve(path, expanded_argv, data->envp_copy);
	free_tokens(expanded_argv);
	handle_exec_error(cmd_name, path);
}

static void	fork_and_execute(t_data *data, t_command *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		data->last_exit_status = 1;
		close_redirection_fds(cmd);
		return ;
	}
	if (pid == 0)
		execute_command_internal(data, cmd);
	else
		parent_wait(data, pid, cmd);
}

void	execute_command(t_data *data, t_command *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		if (cmd)
			validate_redirections(data, cmd);
		close_redirection_fds(cmd);
		return ;
	}
	if (handle_simple_builtin(data, cmd))
		return ;
	fork_and_execute(data, cmd);
}
