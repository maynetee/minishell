/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:48:03 by mteichma          #+#    #+#             */
/*   Updated: 2025/05/01 00:11:40 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_all_digits(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	perform_exit_cleanup(t_data *data, char **tokens, int exit_code)
{
	free_tokens(tokens);
	free(data->current_line);
	free_env(data->envp_copy);
	if (data->interactive_mode)
		rl_clear_history();
	exit(exit_code);
}

static void	exit_with_numeric_arg(t_data *data, char **tokens)
{
	int	exit_code;

	if (!is_all_digits(tokens[1]))
	{
		if (data->interactive_mode)
			ft_putendl_fd("exit", 2);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(tokens[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit_code = 2;
	}
	else if (tokens[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		data->last_exit_status = 1;
		return ;
	}
	else
	{
		if (data->interactive_mode)
			ft_putendl_fd("exit", 2);
		exit_code = (unsigned char)ft_atoi(tokens[1]);
	}
	perform_exit_cleanup(data, tokens, exit_code);
}

void	builtin_exit(t_data *data, char **tokens)
{
	int	exit_code;

	exit_code = data->last_exit_status;
	if (tokens[1])
	{
		exit_with_numeric_arg(data, tokens);
		return ;
	}
	if (data->interactive_mode)
		ft_putendl_fd("exit", 2);
	free_tokens(tokens);
	free(data->current_line);
	free_env(data->envp_copy);
	if (data->interactive_mode)
		rl_clear_history();
	exit(exit_code);
}
