/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 23:20:19 by mteichma          #+#    #+#             */
/*   Updated: 2025/05/01 00:11:23 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_eacces_error(const char *cmd_name,
		const char *exec_path_attempted)
{
	struct stat	statbuf;

	(void)cmd_name;
	if (exec_path_attempted && stat(exec_path_attempted, &statbuf) == 0
		&& S_ISDIR(statbuf.st_mode))
	{
		ft_putstr_fd((char *)exec_path_attempted, 2);
		ft_putendl_fd(": is a directory", 2);
	}
	else
	{
		if (exec_path_attempted)
			perror((char *)exec_path_attempted);
		else
			perror("permission denied");
	}
	return (126);
}

void	handle_no_path(const char *cmd_name)
{
	if (!ft_strchr(cmd_name, '/'))
	{
		if (errno != ENOENT)
			errno = ENOENT;
		handle_exec_error(cmd_name, NULL);
	}
	else
		handle_exec_error(cmd_name, cmd_name);
}

static void	exec_error_noent(const char *cmd_name,
		const char *exec_path_attempted)
{
	if (exec_path_attempted == NULL)
	{
		ft_putstr_fd((char *)cmd_name, 2);
		ft_putendl_fd(": command not found", 2);
	}
	else
	{
		perror((char *)exec_path_attempted);
	}
}

static void	exec_error_default(const char *cmd_name,
		const char *exec_path_attempted)
{
	if (exec_path_attempted)
		perror((char *)exec_path_attempted);
	else
		perror((char *)cmd_name);
}

void	handle_exec_error(const char *cmd_name, const char *exec_path_attempted)
{
	int	exit_code;

	ft_putstr_fd("minishell: ", 2);
	if (errno == ENOENT)
	{
		exec_error_noent(cmd_name, exec_path_attempted);
		exit_code = 127;
	}
	else if (errno == EACCES)
	{
		exit_code = handle_eacces_error(cmd_name, exec_path_attempted);
	}
	else
	{
		exec_error_default(cmd_name, exec_path_attempted);
		exit_code = 126;
	}
	exit(exit_code);
}
