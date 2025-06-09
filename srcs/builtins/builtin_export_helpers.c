/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:50:46 by mteichma          #+#    #+#             */
/*   Updated: 2025/06/04 21:42:34 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	compute_key_len(const char *arg, int *key_len, int *append)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
		*key_len = equal_sign - arg;
	else
		*key_len = ft_strlen(arg);
	*append = 0;
	if (*key_len > 0 && arg[*key_len - 1] == '+' && equal_sign)
	{
		*append = 1;
		(*key_len)--;
	}
	if (*key_len == 0 && equal_sign)
	{
		ft_putstr_fd("minishell: export: `=': not a valid identifier\n", 2);
		return (-1);
	}
	if (*key_len == 0)
		return (0);
	return (1);
}

static int	validate_key(const char *arg, char *key)
{
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd((char *)arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		free(key);
		return (0);
	}
	return (1);
}

int	print_export_format(char *env_var)
{
	char	*equal_sign;
	int		key_len;

	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign)
	{
		key_len = equal_sign - env_var;
		if (ms_write(1, "declare -x ", 11) < 0 || ms_write(1, env_var,
				key_len) < 0 || ms_write(1, "=\"", 2) < 0 || ms_write(1,
				equal_sign + 1, ft_strlen(equal_sign + 1)) < 0 || ms_write(1,
				"\"\n", 2) < 0)
			return (-1);
	}
	else
	{
		if (ms_write(1, "declare -x ", 11) < 0 || ms_write(1, env_var,
				ft_strlen(env_var)) < 0 || ms_write(1, "\n", 1) < 0)
			return (-1);
	}
	return (0);
}

int	is_valid_export_key(char *env_var)
{
	char	*eq;
	char	*key;
	int		key_len;
	int		ok;

	eq = ft_strchr(env_var, '=');
	if (eq)
		key_len = eq - env_var;
	else
		key_len = ft_strlen(env_var);
	key = ft_substr(env_var, 0, key_len);
	if (!key)
		return (0);
	ok = is_valid_identifier(key);
	free(key);
	return (ok);
}

int	handle_env_var(t_data *data, const char *arg)
{
	int		res;
	int		key_len;
	int		append;
	char	*key;

	res = compute_key_len(arg, &key_len, &append);
	if (res < 0)
		return (1);
	if (res == 0)
		return (0);
	key = ft_substr(arg, 0, key_len);
	if (!key)
		return (1);
	if (!validate_key(arg, key))
		return (1);
	free(key);
	return (update_or_add_env(data, arg, key_len, append));
}
