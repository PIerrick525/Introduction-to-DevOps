/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** exit
*/

#include "mysh.h"

int builtin_exit(char **args, shell_t *shell)
{
    int code;

    if (args[1])
        code = my_atoi(args[1]);
    else
        code = shell->last_exit;
    env_free_array(args);
    free(shell->line);
    env_free(shell->env);
    exit(code);
}
