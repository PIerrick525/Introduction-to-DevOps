/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** run_line
*/

#include "mysh.h"

static int is_empty(const char *str)
{
    int i = 0;

    while (str[i] == ' ' || str[i] == '\t')
        i++;
    return str[i] == '\0';
}

int run_line(char *line, shell_t *shell)
{
    char **segments = split_on(line, ';');
    int ret = 0;
    int i = 0;

    if (!segments)
        return 84;
    if (my_strcmp(segments[0], "|") == 0){
        my_put_error("Incorrect null command.\n");
        return 84;
    }
    while (segments[i]) {
        if (!is_empty(segments[i])) {
            ret = run_pipeline(segments[i], shell);
            shell->last_exit = ret;
        }
        i++;
    }
    free_tokens(segments);
    return ret;
}
