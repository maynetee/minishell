/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 01:34:21 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/30 17:44:14 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:41:10 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/30 14:22:00 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (new_size <= old_size)
		return (ptr);
	if (new_size < old_size * 2)
		new_size = old_size * 2 + 1;
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	copy_size = old_size;
	if (ptr && copy_size)
		ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}
