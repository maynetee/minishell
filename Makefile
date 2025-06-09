NAME = minishell

CC = clang
CFLAGS = -Wall -Wextra -Werror -Iincludes -ILibft/includes

LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a
READLINE_FLAGS = -lreadline

SRC_DIR = srcs
OBJ_DIR = objs

SRCS = \
    srcs/data.c \
    srcs/env_utils.c \
    srcs/execution.c \
    srcs/execution_errors.c \
    srcs/execution_redir.c \
    srcs/execution_redir_check.c \
    srcs/execution_sequence.c \
    srcs/execution_utils.c \
    srcs/expansion.c \
    srcs/expansion_parser.c \
    srcs/expansion_utils.c \
    srcs/heredoc.c \
    srcs/heredoc_cleanup.c \
    srcs/heredoc_input.c \
    srcs/heredoc_setup.c \
    srcs/heredoc_utils.c \
    srcs/line_processor.c \
    srcs/main.c \
    srcs/parser.c \
    srcs/parser_output.c \
    srcs/parser_pipe.c \
    srcs/parser_pipeline_utils.c \
    srcs/parser_redir.c \
    srcs/parser_syntax.c \
    srcs/parser_utils.c \
    srcs/path_handler.c \
    srcs/pipeline_execute.c \
    srcs/pipeline_execute_helpers.c \
    srcs/quotes_utils.c \
    srcs/shell_loops.c \
    srcs/signals.c \
    srcs/signals_utils.c \
srcs/status_utils.c \
srcs/safe_write.c \
srcs/tokenizer.c \
    srcs/tokenizer_state.c \
    srcs/tokenizer_utils.c \
    srcs/builtins/builtin_cd.c \
    srcs/builtins/builtin_cd_helpers.c \
    srcs/builtins/builtin_dispatcher.c \
    srcs/builtins/builtin_echo.c \
    srcs/builtins/builtin_env.c \
    srcs/builtins/builtin_exit.c \
    srcs/builtins/builtin_export.c \
    srcs/builtins/builtin_export_helpers.c \
    srcs/builtins/builtin_pwd.c \
    srcs/builtins/builtin_unset.c \
    srcs/builtins/builtin_unset_utils.c \
    srcs/builtins/export_env_ops.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(READLINE_FLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
