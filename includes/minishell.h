/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:10:26 by mteichma          #+#    #+#             */
/*   Updated: 2025/06/09 01:56:28 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
# endif

# ifndef ECHOCTL
#  define ECHOCTL 0001000
# endif

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal_received;

typedef enum e_state
{
	GENERAL,
	IN_SQ,
	IN_DQ
}								t_state;

typedef struct s_tok_ctx
{
	char						*buffer;
	t_list						*list;
}								t_tok_ctx;

typedef struct s_data
{
	char						**envp_copy;
	int							last_exit_status;
	char						*current_line;
	int							interactive_mode;
}								t_data;

typedef struct s_heredoc_spec
{
	char						*delimiter;
	int							expand;
	struct s_heredoc_spec		*next;
}								t_heredoc_spec;

typedef struct s_heredoc_params
{
	t_data						*data;
	const char					*delimiter;
	int							expand;
	int							write_fd;
}								t_heredoc_params;

typedef struct s_command
{
	char						**argv;
	char						*infile;
	char						*outfile;
	t_heredoc_spec				*heredocs;
	int							heredoc_fd;
	int							append_mode;
	struct s_command			*next;
}								t_command;

typedef struct s_sequence
{
	t_command					*pipeline_head;
	struct s_sequence			*next;
}								t_sequence;

typedef struct s_parser_state
{
	t_data						*data;
	t_list						*tok_ptr;
	t_sequence					*seq_head;
	t_sequence					*current_seq;
	t_command					*current_cmd;
	t_list						*args_list;
}								t_parser_state;

char							**copy_env(char **envp);
void							free_env(char **env);
void							increment_shlvl(t_data *data);
void							ensure_pwd(t_data *data);
void							ensure_underscore(t_data *data);
t_command						*create_new_command(void);
void							free_sequence_list(t_sequence *seq_list);
char							*find_env_var_value(char **envp,
									const char *var_name);
void							free_tokens(char **tokens);
void							update_exit_status(t_data *data, int status);

t_list							*tokenize(char *line);
char							**convert_token_list_to_array(t_list *list);
int								finalize_and_check(t_state state, char **buffer,
									t_list **list);
void							append_char_to_buffer(char **str, char c);
void							finalize_token(char **buffer, t_list **list);
int								handle_general_state(char c, char **buffer,
									t_list **list, t_state *state);
int								handle_sq_state(char c, char **buffer,
									t_state *state);
int								handle_dq_state(char c, char **buffer,
									t_state *state, char next_char);
int								dispatch_to_state_handler(char current_char,
									char next_char, t_tok_ctx *ctx,
									t_state *state);

t_sequence						*parse_input_line(t_data *data, t_list *tokens);
void							handle_pipe_token(t_command **current_cmd,
									t_list **args_list);
void							process_parser_token(t_data *data,
									t_command *cmd, t_list **args,
									t_list **tok_ptr);
int								check_syntax(t_list *tok, t_list **args,
									t_sequence *seq_head);
void							handle_parser_output(t_command *cmd,
									t_list **tok);
void							handle_parser_input(t_command *cmd,
									t_list **tok);
void							handle_parser_heredoc(t_command *cmd,
									t_list **tok);
int								check_first_token(t_list *token);
int								start_new_command(t_parser_state *s);
int								handle_regular_token(t_parser_state *s,
									char *tok);

char							*perform_expansion(char *token, t_data *data);
void							append_char_to_result(char **result, char c);
void							perform_dollar_expansion(char **result,
									char *var_name, t_data *data);
void							update_expansion_state(char c,
									t_state *current_state);
void							get_var_name_and_consumption(const char *token,
									int pos, char **var_name, int *consumed);

void							execute_command_internal(t_data *data,
									t_command *cmd);
void							execute_command(t_data *data, t_command *cmd);
int								handle_simple_builtin(t_data *data,
									t_command *cmd);
void							execute_sequences(t_data *data,
									t_sequence *seq_list);
char							*find_command_in_path(t_data *data,
									const char *cmd);
char							**expand_argv(t_data *data, char **raw_argv);

void							handle_exec_error(const char *cmd_name,
									const char *path);
void							handle_no_path(const char *cmd_name);

void							apply_redirections(t_command *cmd);
void							close_redirection_fds(t_command *cmd);
int								validate_redirections(t_data *data,
									t_command *cmd);
void							execute_pipeline(t_data *data,
									t_command *cmd_list);
pid_t							spawn_cmd(t_data *data, t_command *cmd,
									int *in_fd_ptr);

int								handle_heredocs(t_data *data,
									t_command *cmd_list);
int								read_heredoc_input(t_data *data,
									const char *delim, int expand,
									int write_fd);
int								handle_heredoc_line(t_heredoc_params *params,
									char *line);
int								setup_heredoc_io(int *stdin_backup);
void							restore_stdin(int stdin_backup);
void							cleanup_after_heredoc(int stdin_backup);
char							*remove_quotes(const char *str);
int								ms_write(int fd, const char *buf, size_t len);

int								dispatch_builtin(t_data *data, char **tokens);
int								is_builtin(char *cmd);
int								execute_builtin(t_data *data, t_command *cmd);
int								builtin_cd(t_data *data, char **tokens);
int								builtin_echo(t_data *data, char **tokens);
void							builtin_exit(t_data *data, char **tokens);
int								builtin_pwd(t_data *data);
int								builtin_env(t_data *data, char **tokens);
int								builtin_unset(t_data *data, char **tokens);
char							*expand_tilde_slash(t_data *data,
									const char *arg);
char							*get_home_target(t_data *data);
char							*get_oldpwd_target(t_data *data);
int								is_valid_identifier(const char *str);
int								builtin_export(t_data *data, char **tokens);
int								print_export_format(char *env_var);
int								is_valid_export_key(char *env_var);
int								handle_env_var(t_data *data, const char *arg);
int								find_env_index(char **envp, const char *key,
									int key_len);
int								update_or_add_env(t_data *data, const char *arg,
									int key_len, int append);

void							clear_heredoc_list(t_command *cmd);

void							setup_main_signal_handlers(void);
void							setup_child_signal_handlers(void);
void							setup_heredoc_signals(void);
void							ignore_signals(void);

void							run_line(t_data *data, char *line);
void							interactive_loop(t_data *data);
void							non_interactive_loop(t_data *data);
void							clean_exit(t_data *data, int exit_code);

#endif
