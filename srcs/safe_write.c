/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:50:34 by mteichma          #+#    #+#             */
/*   Updated: 2025/06/04 18:50:35 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_write(int fd, const char *buf, size_t len)
{
	if (write(fd, buf, len) < 0)
		return (-1);
	return (0);
}
