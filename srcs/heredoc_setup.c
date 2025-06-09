/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 03:57:03 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 16:55:52 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_stdin(int stdin_backup)
{
	if (stdin_backup < 0)
		return ;
	if (dup2(stdin_backup, 0) < 0)
		perror("minishell: dup2 restore stdin failed");
	close(stdin_backup);
}

static int	get_terminal_fd_fallback(void)
{
	int	term_in_fd;

	term_in_fd = dup(0);
	if (term_in_fd < 0)
		perror("minishell: dup fallback stdin failed");
	return (term_in_fd);
}

static int	get_terminal_input_fd(int *stdin_backup)
{
	int		term_in_fd;
	char	*tty_name;

	*stdin_backup = dup(0);
	if (*stdin_backup < 0)
	{
		perror("minishell: dup stdin backup failed");
		return (-1);
	}
	tty_name = ttyname(0);
	if (tty_name)
		term_in_fd = open(tty_name, O_RDONLY);
	else
		term_in_fd = -1;
	if (term_in_fd < 0)
	{
		term_in_fd = get_terminal_fd_fallback();
		if (term_in_fd < 0)
		{
			close(*stdin_backup);
			*stdin_backup = -1;
			return (-1);
		}
	}
	return (term_in_fd);
}

static void	restore_stdin_on_failure(int stdin_backup)
{
	if (stdin_backup < 0)
		return ;
	if (dup2(stdin_backup, 0) < 0)
		perror("minishell: dup2 restore stdin failed");
	close(stdin_backup);
}

int	setup_heredoc_io(int *stdin_backup)
{
	int	term_in_fd;

	term_in_fd = get_terminal_input_fd(stdin_backup);
	if (term_in_fd < 0)
		return (0);
	if (dup2(term_in_fd, 0) < 0)
	{
		perror("minishell: dup2 term_in_fd failed");
		close(term_in_fd);
		restore_stdin_on_failure(*stdin_backup);
		*stdin_backup = -1;
		return (0);
	}
	if (term_in_fd != 0)
		close(term_in_fd);
	return (1);
}
