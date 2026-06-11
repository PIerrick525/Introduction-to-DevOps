/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** main_bonus
*/

#include "mysh_bonus.h"

static int read_line_bonus(char **line, size_t *n)
{
    ssize_t ret;

    if (isatty(STDIN_FILENO))
        my_printf("%s", PROMPT);
    ret = getline(line, n, stdin);
    if (ret == -1)
        return -1;
    if ((*line)[ret - 1] == '\n')
        (*line)[ret - 1] = '\0';
    return (int)ret;
}

static int loop_bonus(shell_t *shell)
{
    size_t n = 0;

    shell->line = NULL;
    while (1) {
        if (read_line_bonus(&shell->line, &n) == -1)
            break;
        if (shell->line[0] != '\0')
            shell->last_exit = run_line_bonus(shell->line, shell);
    }
    free(shell->line);
    return shell->last_exit;
}

int main(int ac, char **av, char **envp)
{
    shell_t shell;
    int ret;

    (void)ac;
    (void)av;
    shell.env = env_copy(envp);
    shell.last_exit = 0;
    shell.line = NULL;
    ret = loop_bonus(&shell);
    env_free(shell.env);
    return ret;
}
