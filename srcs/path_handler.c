/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:23:17 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 18:21:05 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*build_full_path(char *dir, const char *cmd)
{
	char	*temp;
	char	*full;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full = ft_strjoin(temp, cmd);
	free(temp);
	if (!full)
		return (NULL);
	if (!access(full, X_OK))
		return (full);
	free(full);
	return (NULL);
}

static char	*search_in_path_dirs(char **dirs, const char *cmd)
{
	char	*exec_path;
	int		i;

	i = 0;
	while (dirs[i])
	{
		exec_path = build_full_path(dirs[i], cmd);
		if (exec_path)
			return (exec_path);
		i++;
	}
	return (NULL);
}

char	*find_command_in_path(t_data *data, const char *cmd)
{
	char	*path_value;
	char	**dirs;
	char	*exec_path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (!access(cmd, X_OK))
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_value = find_env_var_value(data->envp_copy, "PATH");
	if (!path_value || *path_value == '\0')
	{
		errno = ENOENT;
		return (NULL);
	}
	dirs = ft_split(path_value, ':');
	if (!dirs)
		return (NULL);
	exec_path = search_in_path_dirs(dirs, cmd);
	free_tokens(dirs);
	return (exec_path);
}
