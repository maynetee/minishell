/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:21:42 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/24 22:36:39 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_env_var_value(char **envp, const char *v_name)
{
	size_t	name_len;
	int		i;

	if (!envp || !v_name)
		return (NULL);
	name_len = ft_strlen(v_name);
	i = 0;
	while (envp && envp[i])
	{
		if (!ft_strncmp(envp[i], v_name, name_len) && envp[i][name_len] == '=')
			return (envp[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}
