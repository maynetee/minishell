/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 20:57:34 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 00:37:27 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char **envp)
{
	int		i;
	char	**new_env;

	i = 0;
	while (envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			new_env[i] = NULL;
			free_env(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
}

void	increment_shlvl(t_data *data)
{
	char	*value;
	int		lvl;
	char	*lvl_str;
	char	*entry;

	value = find_env_var_value(data->envp_copy, "SHLVL");
	if (!value)
	{
		handle_env_var(data, "SHLVL=1");
		return ;
	}
	lvl = ft_atoi(value);
	lvl++;
	lvl_str = ft_itoa(lvl);
	if (!lvl_str)
		return ;
	entry = ft_strjoin("SHLVL=", lvl_str);
	free(lvl_str);
	if (!entry)
		return ;
	handle_env_var(data, entry);
	free(entry);
}

void	ensure_pwd(t_data *data)
{
	char	*pwd;
	char	*entry;

	if (find_env_var_value(data->envp_copy, "PWD"))
		return ;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return ;
	entry = ft_strjoin("PWD=", pwd);
	free(pwd);
	if (!entry)
		return ;
	handle_env_var(data, entry);
	free(entry);
}

void	ensure_underscore(t_data *data)
{
	if (find_env_var_value(data->envp_copy, "_"))
		return ;
	handle_env_var(data, "_=/usr/bin/env");
}
