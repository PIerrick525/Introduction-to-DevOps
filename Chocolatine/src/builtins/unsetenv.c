/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** unsetenv
*/

#include "mysh.h"

int builtin_unsetenv(char **args, shell_t *shell)
{
    if (!args[1]){
        my_put_error("unsetenv: Too few arguments.\n");
        return 84;
    }
    env_unset(&shell->env, args[1]);
    return 0;
}
