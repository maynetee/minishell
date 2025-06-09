/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:34:38 by mteichma          #+#    #+#             */
/*   Updated: 2025/05/01 00:36:50 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_tilde_slash(t_data *data, const char *arg)
{
	char	*home;
	char	*res;

	home = find_env_var_value(data->envp_copy, "HOME");
	if (!home || !*home)
		return (ft_putendl_fd("minishell: cd: HOME not set", 2), NULL);
	res = ft_strjoin(home, arg + 1);
	return (res);
}

char	*get_home_target(t_data *data)
{
	char	*home;

	home = find_env_var_value(data->envp_copy, "HOME");
	if (!home || !*home)
		return (ft_putendl_fd("minishell: cd: HOME not set", 2), NULL);
	return (home);
}

char	*get_oldpwd_target(t_data *data)
{
	char	*old;

	old = find_env_var_value(data->envp_copy, "OLDPWD");
	if (!old || !*old)
		return (ft_putendl_fd("minishell: cd: OLDPWD not set", 2), NULL);
	ft_putendl_fd(old, 1);
	return (old);
}
