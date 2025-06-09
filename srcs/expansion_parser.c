/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_parser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 23:20:33 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 17:57:13 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_var_char(int c, int is_first)
{
	if (is_first)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

static char	*extract_simple_var_name(const char *start, int *len_read)
{
	int	i;

	*len_read = 0;
	if (!start || !*start)
		return (NULL);
	if (start[0] == '?' || start[0] == '*' || start[0] == '@'
		|| start[0] == '#' || start[0] == '-')
	{
		*len_read = 1;
		return (ft_substr(start, 0, 1));
	}
	if (ft_isdigit((unsigned char)start[0]))
	{
		*len_read = 1;
		return (ft_substr(start, 0, 1));
	}
	if (!is_valid_var_char(start[0], 1))
		return (NULL);
	i = 1;
	while (start[i] && is_valid_var_char(start[i], 0))
		i++;
	*len_read = i;
	if (i == 0)
		return (NULL);
	return (ft_substr(start, 0, i));
}

static char	*extract_braced_var_name(const char *start_after_brace,
		int *len_read)
{
	int		i;
	char	*name;

	*len_read = 0;
	i = 0;
	while (start_after_brace[i] && start_after_brace[i] != '}')
		i++;
	if (start_after_brace[i] != '}' || i == 0)
		return (NULL);
	name = ft_substr(start_after_brace, 0, i);
	if (!name)
		return (NULL);
	if (ft_strcmp(name, "?") != 0
		&& !(i == 1 && (ft_isdigit((unsigned char)name[0]) || name[0] == '*'
				|| name[0] == '@' || name[0] == '#' || name[0] == '-'))
		&& !is_valid_identifier(name))
	{
		free(name);
		return (NULL);
	}
	*len_read = i;
	return (name);
}

void	get_var_name_and_consumption(const char *token, int pos,
		char **var_name, int *consumed)
{
	int	name_len;

	*var_name = NULL;
	*consumed = 1;
	if (token[pos + 1] == '{')
	{
		*var_name = extract_braced_var_name(&token[pos + 2], &name_len);
		if (*var_name)
			*consumed += name_len + 2;
	}
	else
	{
		*var_name = extract_simple_var_name(&token[pos + 1], &name_len);
		if (*var_name)
			*consumed += name_len;
	}
}
