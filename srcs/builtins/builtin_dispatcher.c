/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:22:53 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 17:33:02 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dispatch_builtin(t_data *data, char **tokens)
{
	if (!tokens || !tokens[0])
		return (0);
	if (!ft_strcmp(tokens[0], "pwd"))
		return (data->last_exit_status = builtin_pwd(data), 1);
	if (!ft_strcmp(tokens[0], "echo"))
		return (data->last_exit_status = builtin_echo(data, tokens), 1);
	if (!ft_strcmp(tokens[0], "cd"))
		return (data->last_exit_status = builtin_cd(data, tokens), 1);
	if (!ft_strcmp(tokens[0], "exit"))
		return (builtin_exit(data, tokens), 1);
	if (!ft_strcmp(tokens[0], "export"))
		return (data->last_exit_status = builtin_export(data, tokens), 1);
	if (!ft_strcmp(tokens[0], "unset"))
		return (data->last_exit_status = builtin_unset(data, tokens), 1);
	if (!ft_strcmp(tokens[0], "env"))
		return (data->last_exit_status = builtin_env(data, tokens), 1);
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "env") == 0);
}

static void	execute_builtin_with_redir(t_data *data, t_command *cmd)
{
	char	**expanded_argv;

	apply_redirections(cmd);
	expanded_argv = expand_argv(data, cmd->argv);
	if (!expanded_argv)
		exit(1);
	dispatch_builtin(data, expanded_argv);
	free_tokens(expanded_argv);
	exit(data->last_exit_status);
}

static void	do_redir_exec(t_data *data, t_command *cmd, char **expanded_argv)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		free_tokens(expanded_argv);
		perror("minishell: fork");
		data->last_exit_status = 1;
		return ;
	}
	if (pid == 0)
		execute_builtin_with_redir(data, cmd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->last_exit_status = WEXITSTATUS(status);
}

int	execute_builtin(t_data *data, t_command *cmd)
{
	char	**expanded_argv;

	expanded_argv = expand_argv(data, cmd->argv);
	if (!expanded_argv || !expanded_argv[0])
	{
		free_tokens(expanded_argv);
		return (0);
	}
	if (!is_builtin(expanded_argv[0]))
	{
		free_tokens(expanded_argv);
		return (0);
	}
	if (cmd->infile || cmd->outfile || cmd->heredoc_fd != -1)
		do_redir_exec(data, cmd, expanded_argv);
	else
		dispatch_builtin(data, expanded_argv);
	free_tokens(expanded_argv);
	return (1);
}
