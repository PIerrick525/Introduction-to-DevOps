/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** loop
*/

#include "mysh.h"

static int read_line(char **line, size_t *n)
{
    ssize_t ret;

    if (isatty(STDIN_FILENO))
        my_printf("%s", PROMPT);
    ret = getline(line, n, stdin);
    if (ret == -1)
        return (-1);
    if ((*line)[ret - 1] == '\n')
        (*line)[ret - 1] = '\0';
    return (ret);
}

int loop(shell_t *shell)
{
    size_t n;

    shell->line = NULL;
    n = 0;
    while (1) {
        if (read_line(&shell->line, &n) == -1)
            break;
        if (shell->line[0] != '\0')
            shell->last_exit = run_line(shell->line, shell);
    }
    free(shell->line);
    return (shell->last_exit);
}
