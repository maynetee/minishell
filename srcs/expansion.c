/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:41:15 by mteichma          #+#    #+#             */
/*   Updated: 2025/06/04 18:50:20 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_dollar_sign(const char *token, int *i, char **result,
		t_data *data)
{
	char	*var_name;
	int		consumed;

	get_var_name_and_consumption(token, *i, &var_name, &consumed);
	if (var_name)
	{
		perform_dollar_expansion(result, var_name, data);
		*i += consumed;
	}
	else
	{
		append_char_to_result(result, '$');
		*i += 1;
	}
}

static void	process_expansion_loop(const char *tok, char **res, t_data *data)
{
	int		i;
	t_state	state;
	t_state	prev_state;

	i = 0;
	state = GENERAL;
	while (tok[i])
	{
		if (state == GENERAL && tok[i] == '$' && tok[i + 1] == '\'')
		{
			update_expansion_state('\'', &state);
			i += 2;
			continue ;
		}
		prev_state = state;
		update_expansion_state(tok[i], &state);
		if (state != prev_state)
			i++;
		else if (tok[i] == '$' && state != IN_SQ)
			handle_dollar_sign(tok, &i, res, data);
		else
			append_char_to_result(res, tok[i++]);
	}
}

char	*perform_expansion(char *token, t_data *data)
{
	char	*result;
	char	*p;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	process_expansion_loop(token, &result, data);
	p = result;
	while (*p)
	{
		if (*p == '\a')
			*p = '"';
		p++;
	}
	return (result);
}
