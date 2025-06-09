/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 04:03:56 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 16:53:56 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_after_heredoc(int stdin_backup)
	{
	restore_stdin(stdin_backup);
	rl_done = 0;
	rl_replace_line("", 0);
	rl_on_new_line();
	setup_main_signal_handlers();
}
