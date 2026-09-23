_This project has been created as part of the 42 curriculum by mteichma and lupetit._

# minishell

A small Bash-like shell written in C: an interactive prompt with quoting, variable expansion, redirections, heredocs, pipelines, builtins and signal handling.

## What it does

- **Prompt and input**: `minishell> ` prompt with GNU readline line editing and history. When stdin is not a terminal, commands are read line by line with `get_next_line`, so the shell can be driven by piped input.
- **Quoting**: single quotes disable all interpretation, double quotes disable everything except `$`. An unclosed quote is a syntax error.
- **Expansion**: `$VAR`, `${VAR}`, `$?` (last exit status) and `$0` (expands to `minishell`), in unquoted and double-quoted text. `$'...'` is treated as single-quoted text.
- **Redirections**: `<`, `>`, `>>` and `<<` (heredoc). A quoted heredoc delimiter (`<<'EOF'`) disables expansion in the body.
- **Pipelines** of any length (`cmd1 | cmd2 | cmd3`), one process per stage. Commands are resolved through `PATH`, or run directly when they contain a `/`.
- **Builtins**: `echo` (with `-n`, including `-nnn`), `cd` (no argument, `~`, `~/path`, `-`; updates `PWD` and `OLDPWD`), `pwd`, `export` (sorted `declare -x` listing, `NAME=value`, `NAME+=value`, identifier validation), `unset`, `env`, `exit` (numeric argument taken modulo 256).
- **Environment**: the shell works on its own copy of `envp`; at startup it increments `SHLVL` and sets `PWD` and `_` if missing.
- **Signals**: Ctrl-C shows a fresh prompt, Ctrl-D exits, Ctrl-\ is ignored at the prompt. Running children get the default handlers, and the shell prints `Quit` when a child is killed by SIGQUIT.
- **Exit statuses**: 127 (command not found), 126 (not executable, or a directory), 2 (syntax error, invalid `exit` argument), 128 + signal number for a child killed by a signal. A pipeline returns the status of its last command.

## How it works

```
readline() / get_next_line()
        |
        v
tokenize()             tokenizer*.c    quote-aware state machine -> list of words and operators
        |
        v
parse_input_line()     parser*.c       syntax checks, builds a linked list of t_command
        |                              (argv, infile, outfile + append flag, heredoc list)
        v
execute_sequences()    execution_sequence.c
        |-- handle_heredocs()          read every heredoc of the pipeline before forking
        |-- execute_command()          single command: builtin in the shell process, or fork + execve
        '-- execute_pipeline()         one fork per stage, pipes chained, wait for all stages
                  |
                  v
            expand_argv()  expansion*.c   $-expansion and quote removal, per command, at run time
```

The grammar is flat (commands, redirections, pipes), so the parser produces a command table (`t_command` list in `includes/minishell.h`) rather than a tree. A `t_sequence` wrapper exists, but the parser always produces a single pipeline per line.

The tokenizer (`srcs/tokenizer_state.c`) is a three-state machine (`GENERAL`, `IN_SQ`, `IN_DQ`). It splits on unquoted blanks and on `|`, `<`, `>`, `<<`, `>>`, and keeps quote characters inside the token. Expansion (`srcs/expansion.c`) runs the same state machine again at execution time: it decides whether each `$` expands and which quote characters are removed, in a single pass.

In a pipeline, `spawn_cmd()` (`srcs/pipeline_execute_helpers.c`) creates a pipe for every stage except the last. The child `dup2`s the previous read end and the new write end onto stdin/stdout. The parent closes the write end and keeps the read end as the next stage's input. It then waits for every child and takes the exit status from the last one.

## Build and run

Requirements: `clang`, `make`, GNU readline with development headers (e.g. `libreadline-dev` on Debian/Ubuntu). Libft (bundled, with `ft_printf` and `get_next_line`) is built automatically.

```sh
make            # builds Libft, then ./minishell
./minishell     # interactive mode
printf 'echo hello | tr a-z A-Z\n' | ./minishell   # non-interactive mode
make clean      # remove object files
make fclean     # also remove the binary and libft.a
make re         # full rebuild
```

On macOS, the system readline (libedit) lacks `rl_replace_line`. Build against Homebrew's GNU readline instead:

```sh
make CFLAGS="-Wall -Wextra -Werror -include stdio.h -Iincludes -ILibft/includes -I$(brew --prefix readline)/include" \
     READLINE_FLAGS="-L$(brew --prefix readline)/lib -lreadline"
```

## Design choices and hard parts

- **Quotes kept until expansion** (`srcs/tokenizer_state.c`, `srcs/expansion.c`). Removing quotes during tokenization would lose the information about which `$` must expand. Keeping them lets one pass handle mixed words such as `"$USER"'$USER'x`.
- **Heredocs through pipes, before any fork** (`srcs/heredoc.c`). All heredocs of a pipeline are read up front, in order, each into a pipe instead of a temporary file, so there is nothing to clean up on disk. Only the last heredoc of a command keeps its read end. Input comes from the controlling terminal (`ttyname(0)` in `srcs/heredoc_setup.c`), with stdin saved and restored around the read.
- **Ctrl-C inside a heredoc** (`srcs/signals.c`, `srcs/heredoc_cleanup.c`). While readline waits for input, the SIGINT handler sets `rl_done` and closes fd 0 so the read returns at once. The shell then restores stdin from its backup and reinstalls the prompt handlers. The only global variable is `volatile sig_atomic_t g_signal_received`, as the subject requires.
- **Where builtins run** (`srcs/execution_utils.c`, `srcs/builtins/builtin_dispatcher.c`). A builtin without redirections runs in the shell process, so `cd`, `export`, `unset` and `exit` affect the shell itself. With a redirection, it runs in a forked child so the shell's own file descriptors are never rewired. `exit` is special-cased in `parent_wait()` (`srcs/execution.c`) so it still terminates the shell.
- **Signal-aware waiting** (`srcs/execution.c`, `srcs/status_utils.c`). The parent ignores SIGINT and SIGQUIT while it waits, so only the foreground child is interrupted. The exit status is then rebuilt as 128 + signal number, and a core dump is detected from the wait status.
- The 42 Norm (at most 25 lines per function and 5 functions per file, no `for` loops) explains the many small source files.

## Limitations

- Out of scope for the mandatory part and not implemented: `;`, `&&`, `||`, parentheses, `*` wildcards, backslash escapes, job control.
- No word splitting after expansion: with `A="-n hi"`, `echo $A` receives a single argument. An unquoted variable that expands to nothing in command position gives `: command not found` (127) instead of being dropped.
- Redirection targets are used as written, with no expansion and no quote removal: `> "out"` creates a file literally named `"out"`. Only the last `<` and the last `>`/`>>` of a command are kept, so earlier output files are not created.
- A builtin combined with a redirection runs in a child, so `cd dir > f` or `export X=1 > f` does not change the shell's state.
- An expanding heredoc body goes through the same routine as arguments, so quote characters in it are consumed (`it's` becomes `its`). A heredoc body larger than the kernel pipe buffer (64 KiB on Linux) would block, because it is written before the reader starts. Heredocs only work in interactive mode.
- A few exit statuses differ from Bash: a missing input file in a forked command returns 127 (Bash: 1), and Ctrl-C during a heredoc leaves `$?` at 1 (Bash: 130). `export NAME` without `=` is accepted but not stored.

## Resources

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)
- Man pages: `fork(2)`, `execve(2)`, `pipe(2)`, `dup2(2)`, `waitpid(2)`, `sigaction(2)`, `readline(3)`
