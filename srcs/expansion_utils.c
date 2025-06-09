/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:35:10 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/30 17:44:53 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_char_to_result(char **result, char c)
{
	char	*temp;
	size_t	old_len;

	old_len = 0;
	if (*result)
		old_len = ft_strlen(*result);
	temp = ft_realloc(*result, old_len, old_len + 2);
	if (!temp)
	{
		free(*result);
		*result = NULL;
		return ;
	}
	*result = temp;
	(*result)[old_len] = c;
	(*result)[old_len + 1] = '\0';
}

static void	append_str_to_result(char **result, const char *str)
{
	char	*temp;

	if (!str)
		return ;
	if (!*result)
		temp = ft_strdup(str);
	else
		temp = ft_strjoin(*result, str);
	if (!temp)
	{
		if (*result)
			free(*result);
		*result = NULL;
		return ;
	}
	if (*result)
		free(*result);
	*result = temp;
}

static void	expand_status(char **result, t_data *data)
{
	char	*status_str;

	status_str = ft_itoa(data->last_exit_status);
	if (status_str)
	{
		append_str_to_result(result, status_str);
		free(status_str);
	}
	else
		append_str_to_result(result, "0");
}

void	perform_dollar_expansion(char **result, char *var_name, t_data *data)
{
	char	*var_value;

	if (!var_name)
		return ;
	if (!ft_strcmp(var_name, "?"))
		expand_status(result, data);
	else if (!ft_strcmp(var_name, "0"))
		append_str_to_result(result, "minishell");
	else
	{
		var_value = find_env_var_value(data->envp_copy, var_name);
		if (var_value)
			append_str_to_result(result, var_value);
	}
	free(var_name);
}

void	update_expansion_state(char c, t_state *current_state)
{
	if (c == '\'' && *current_state == GENERAL)
		*current_state = IN_SQ;
	else if (c == '"' && *current_state == GENERAL)
		*current_state = IN_DQ;
	else if ((c == '\'' && *current_state == IN_SQ) || (c == '"'
			&& *current_state == IN_DQ))
		*current_state = GENERAL;
}
