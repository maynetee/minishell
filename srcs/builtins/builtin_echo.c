/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:47:42 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 01:57:25 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n_option(char *token)
{
	int	i;

	if (!token || token[0] != '-' || token[1] != 'n')
		return (0);
	i = 2;
	while (token[i] == 'n')
		i++;
	return (token[i] == '\0');
}

int	builtin_echo(t_data *data, char **tokens)
{
	int	print_nl;
	int	i;

	(void)data;
	print_nl = 1;
	i = 1;
	while (tokens[i] && is_n_option(tokens[i]))
	{
		print_nl = 0;
		i++;
	}
	while (tokens[i])
	{
		if (ms_write(1, tokens[i], ft_strlen(tokens[i])) < 0)
			return (perror("minishell: echo"), 1);
		if (tokens[i + 1])
			if (ms_write(1, " ", 1) < 0)
				return (perror("minishell: echo"), 1);
		i++;
	}
	if (print_nl)
		if (ms_write(1, "\n", 1) < 0)
			return (perror("minishell: echo"), 1);
	return (0);
}
