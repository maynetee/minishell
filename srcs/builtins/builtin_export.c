/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 01:22:04 by lupetit           #+#    #+#             */
/*   Updated: 2025/05/01 00:32:46 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	bubble_sort_env(char **arr, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static int	print_sorted_env(char **env_copy, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (env_copy[i][0] == '_' && (env_copy[i][1] == '='
				|| env_copy[i][1] == '\0'))
		{
			i++;
			continue ;
		}
		if (is_valid_export_key(env_copy[i]))
		{
			if (print_export_format(env_copy[i]) < 0)
				return (-1);
		}
		i++;
	}
	return (0);
}

static int	sort_and_print_env(char **env_copy, int count)
{
	bubble_sort_env(env_copy, count);
	return (print_sorted_env(env_copy, count));
}

static int	handle_export_no_args(t_data *data)
{
	int		count;
	char	**env_copy_to_sort;

	count = 0;
	if (data->envp_copy)
		while (data->envp_copy[count])
			count++;
	if (!count)
		return (0);
	env_copy_to_sort = copy_env(data->envp_copy);
	if (!env_copy_to_sort)
	{
		ft_putendl_fd("minishell: export: malloc failed", 2);
		return (1);
	}
	if (sort_and_print_env(env_copy_to_sort, count) < 0)
	{
		free_env(env_copy_to_sort);
		return (1);
	}
	free_env(env_copy_to_sort);
	return (0);
}

int	builtin_export(t_data *data, char **tokens)
{
	int	i;
	int	exit_status;

	if (!tokens[1])
		return (handle_export_no_args(data));
	i = 1;
	exit_status = 0;
	while (tokens[i])
	{
		if (handle_env_var(data, tokens[i]))
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
