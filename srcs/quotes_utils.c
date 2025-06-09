/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 00:14:38 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 16:55:55 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(const char *str)
{
	size_t	len;
	char	quote_char;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	quote_char = str[0];
	if ((quote_char == '\'' || quote_char == '"') && str[len - 1] == quote_char)
		return (ft_substr(str, 1, len - 2));
	else
		return (ft_strdup(str));
}
