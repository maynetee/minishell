/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:18:18 by mteichma          #+#    #+#             */
/*   Updated: 2025/06/04 18:51:08 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_cd_target(t_data *data, char **toks, int *need_free)
{
	*need_free = 0;
	if (!toks[1] || !ft_strcmp(toks[1], "~"))
		return (get_home_target(data));
	if (!ft_strcmp(toks[1], "-"))
		return (get_oldpwd_target(data));
	if (toks[1][0] == '~' && toks[1][1] == '/')
	{
		*need_free = 1;
		return (expand_tilde_slash(data, toks[1]));
	}
	return (toks[1]);
}

static void	update_pwd_oldpwd_env(t_data *data, char *oldcwd)
{
	char	*entry;
	char	*cwd;

	entry = ft_strjoin("OLDPWD=", oldcwd);
	if (entry)
	{
		handle_env_var(data, entry);
		free(entry);
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		entry = ft_strjoin("PWD=", cwd);
		if (entry)
		{
			handle_env_var(data, entry);
			free(entry);
		}
		free(cwd);
	}
}

static int	chdir_and_update(t_data *data, char *target, char *oldcwd)
{
	if (chdir(target) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(target);
		return (1);
	}
	update_pwd_oldpwd_env(data, oldcwd);
	return (0);
}

static char	*resolve_oldcwd(t_data *data, int *oldfree)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	*oldfree = 1;
	if (!cwd)
	{
		ft_putstr_fd("minishell: cd: error retrieving current directory: ", 2);
		perror("getcwd");
		cwd = (char *)find_env_var_value(data->envp_copy, "PWD");
		*oldfree = 0;
		if (!cwd)
			cwd = "";
	}
	return (cwd);
}

int	builtin_cd(t_data *data, char **tokens)
{
	char	*oldcwd;
	char	*target;
	int		status;
	int		to_free;
	int		oldfree;

	if (tokens[1] && tokens[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	oldcwd = resolve_oldcwd(data, &oldfree);
	target = get_cd_target(data, tokens, &to_free);
	if (!target)
	{
		if (oldfree)
			free(oldcwd);
		return (1);
	}
	status = chdir_and_update(data, target, oldcwd);
	if (to_free)
		free(target);
	if (oldfree)
		free(oldcwd);
	return (status);
}
