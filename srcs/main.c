/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:10:45 by mteichma          #+#    #+#             */
/*   Updated: 2025/05/01 00:26:35 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

static int	init_data(t_data *data, char **envp)
{
	ft_bzero(data, sizeof(*data));
	data->last_exit_status = 0;
	data->envp_copy = copy_env(envp);
	if (!data->envp_copy)
	{
		perror("minishell: init_data");
		return (1);
	}
	ensure_pwd(data);
	increment_shlvl(data);
	ensure_underscore(data);
	data->interactive_mode = isatty(STDIN_FILENO);
	return (0);
}

void	clean_exit(t_data *data, int exit_code)
{
	free_env(data->envp_copy);
	if (data->interactive_mode)
		rl_clear_history();
	if (data->current_line)
		free(data->current_line);
	exit(exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	if (init_data(&data, envp))
		return (1);
	if (data.interactive_mode)
		interactive_loop(&data);
	else
		non_interactive_loop(&data);
	clean_exit(&data, data.last_exit_status);
	return (0);
}
