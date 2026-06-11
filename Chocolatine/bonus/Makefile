##
## EPITECH PROJECT, 2026
## G-PSU-200-STG-2-1-minishell2
## File description:
## Makefile
##
CC      =   epiclang
SRC     =   main_bonus.c                        \
            run_line_bonus.c                    \
            parse_cmd_bonus.c                   \
            redirect_bonus.c                    \
			utils_bonus.c						\
			init_free_bonus.c					\
            ../src/loop.c                           \
            ../src/executor.c                       \
            ../src/executor_external.c              \
            ../src/path.c                           \
            ../src/environment/env.c                \
            ../src/environment/env_utils.c          \
            ../src/environment/env_operation.c      \
            ../src/environment/create_node.c        \
            ../src/builtins/exit.c                  \
            ../src/builtins/env_cmd.c               \
            ../src/builtins/cd.c                    \
            ../src/builtins/setenv.c                \
            ../src/builtins/unsetenv.c              \
            ../src/parser/split_on.c                \
            ../src/parser/run_line.c                \
            ../src/parser/parse_cmd.c               \
            ../src/parser/utils.c                   \
            ../src/pipes/run_pipeline.c             \
            ../src/pipes/pipe_exec.c                \
            ../src/redirections/redirect.c          \
            ../src/redirections/heredoc.c           \
            ../lib/my/my_str_to_word_array.c        \
            ../lib/my/my_split.c                    \
            ../lib/my/my_put_error.c                \
            ../lib/my/my_atoi.c                     \
            ../lib/my/my_realloc.c                  \
            ../lib/my_printf/flags.c                \
            ../lib/my_printf/flags_utils.c          \
            ../lib/my_printf/my_printf.c            \
            ../lib/my_printf/my_putchar.c           \
            ../lib/my_printf/my_put_double_base.c   \
            ../lib/my_printf/my_put_nbr_auto.c      \
            ../lib/my_printf/my_put_nbr_base.c      \
            ../lib/my_printf/my_put_nbr.c           \
            ../lib/my_printf/my_put_nbr_exp.c       \
            ../lib/my_printf/my_put_nbr_float.c     \
            ../lib/my_printf/my_put_ptr.c           \
            ../lib/my_printf/my_putstr.c            \
            ../lib/my_printf/my_strlen.c            \
            ../lib/my/my_strcmp.c                   \
            ../lib/my_printf/parser.c               \
            ../lib/my_printf/puissance.c            \
            ../lib/my_printf/specifier_base.c       \
            ../lib/my_printf/specifier_basic.c      \
            ../lib/my_printf/specifier_float.c      \
            ../lib/my/my_int_to_str.c               \
            ../lib/my_printf/specifier_float2.c     \
            ../lib/my/my_strdup.c                   \
            ../lib/my_printf/my_strchr.c            \
            ../lib/my/my_strchr_ptr.c               \
            ../lib/my/my_strcpy.c                   \
            ../lib/my_printf/specifier_int.c

CPPFLAGS    =   -I ../include

OBJ         =   $(SRC:.c=.o)

NAME        =   mysh

all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(CPPFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
