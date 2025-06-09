/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env_ops.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 23:21:40 by mteichma          #+#    #+#             */
/*   Updated: 2025/06/04 18:51:45 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_env_index(char **envp, const char *key, int key_len)
{
	int	i;

	i = 0;
	if (!envp || !key)
		return (-1);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, key_len) && (envp[i][key_len] == '='
			|| envp[i][key_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

static char	**alloc_env_with_entry(char **old_envp, const char *arg)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while (old_envp && old_envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = old_envp[i];
		i++;
	}
	new_envp[count] = ft_strdup(arg);
	if (!new_envp[count])
	{
		free(new_envp);
		return (NULL);
	}
	new_envp[count + 1] = NULL;
	return (new_envp);
}

static int	add_env_var(t_data *data, const char *arg)
{
	char	**new_envp;

	new_envp = alloc_env_with_entry(data->envp_copy, arg);
	if (!new_envp)
		return (1);
	free(data->envp_copy);
	data->envp_copy = new_envp;
	return (0);
}

static char	*build_env_entry(t_data *data, const char *key, const char *val,
		int append)
{
	const char	*old;
	char		*tmp;
	char		*res;
	char		*pref;

	if (append)
	{
		old = find_env_var_value(data->envp_copy, key);
		if (!old)
			old = "";
		tmp = ft_strjoin(old, val);
	}
	else
		tmp = ft_strdup(val);
	if (!tmp)
		return (NULL);
	pref = ft_strjoin(key, "=");
	if (!pref)
		return (free(tmp), NULL);
	res = ft_strjoin(pref, tmp);
	free(pref);
	free(tmp);
	return (res);
}

int	update_or_add_env(t_data *data, const char *arg, int key_len, int append)
{
	char	*key;
	char	*entry;
	char	*eq;
	int		env_idx;
	int		res;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (0);
	key = ft_substr(arg, 0, key_len);
	if (!key)
		return (1);
	env_idx = find_env_index(data->envp_copy, key, key_len);
	entry = build_env_entry(data, key, eq + 1, append);
	free(key);
	if (!entry)
		return (1);
	if (env_idx >= 0)
	{
		free(data->envp_copy[env_idx]);
		data->envp_copy[env_idx] = entry;
		return (0);
	}
	res = add_env_var(data, entry);
	return (free(entry), res);
}
