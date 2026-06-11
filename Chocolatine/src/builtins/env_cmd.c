/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** env_cmd
*/

#include "mysh.h"

int builtin_env(char **args, shell_t *shell)
{
    env_t *cur;

    if (args[1]){
        my_put_error("env: Too many arguments.\n");
        return 84;
    }
    cur = shell->env;
    while (cur){
        my_printf("%s=%s\n", cur->key, cur->value);
        cur = cur->next;
    }
    return 0;
}
