/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:23:21 by mteichma          #+#    #+#             */
/*   Updated: 2025/06/09 01:56:19 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_count(char **envp)
{
	int	n;

	n = 0;
	if (!envp)
		return (0);
	while (envp[n])
		n++;
	return (n);
}

static int	find_var_index(char **envp, const char *var)
{
	int		i;
	size_t	len;

	i = 0;
	if (!envp || !var)
		return (-1);
	len = ft_strlen(var);
	while (envp[i])
	{
		if (ft_strchr("=\0", envp[i][len]) && !ft_strncmp(envp[i], var, len))
			return (i);
		i++;
	}
	return (-1);
}

static char	**realloc_envp_for_unset(t_data *data, int target_index)
{
	char	**new_envp;
	int		old_count;
	int		i;
	int		j;

	old_count = env_count(data->envp_copy);
	new_envp = malloc(sizeof(char *) * old_count);
	if (!new_envp)
		return (data->envp_copy);
	i = 0;
	j = 0;
	while (i < old_count)
	{
		if (i == target_index)
			free(data->envp_copy[i]);
		else
			new_envp[j++] = data->envp_copy[i];
		i++;
	}
	new_envp[j] = NULL;
	free(data->envp_copy);
	return (new_envp);
}

static int	handle_unset_arg(t_data *data, const char *arg)
{
	int	target_index;

	if (!ft_strcmp(arg, "_"))
		return (0);
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: unset: `", 2);
		ft_putstr_fd((char *)arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	target_index = find_var_index(data->envp_copy, arg);
	if (target_index != -1)
		data->envp_copy = realloc_envp_for_unset(data, target_index);
	return (0);
}

int	builtin_unset(t_data *data, char **tokens)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	while (tokens[i])
	{
		if (handle_unset_arg(data, tokens[i]))
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
