/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** executor
*/

#include "mysh.h"

int is_builtin(const char *cmd)
{
    const char *builtins[] = {"cd", "setenv", "unsetenv", "env", "exit", NULL};
    int i;

    i = 0;
    while (builtins[i]){
        if (my_strcmp(cmd, builtins[i]) == 0)
            return (1);
        i++;
    }
    return (0);
}

int run_builtin(char **args, shell_t *shell)
{
    if (my_strcmp(args[0], "cd") == 0)
        return (builtin_cd(args, shell));
    if (my_strcmp(args[0], "setenv") == 0)
        return (builtin_setenv(args, shell));
    if (my_strcmp(args[0], "unsetenv") == 0)
        return (builtin_unsetenv(args, shell));
    if (my_strcmp(args[0], "env") == 0)
        return (builtin_env(args, shell));
    if (my_strcmp(args[0], "exit") == 0)
        return (builtin_exit(args, shell));
    return 84;
}

int execute(char **args, shell_t *shell)
{
    if (!args || !args[0])
        return 0;
    if (is_builtin(args[0]))
        return (run_builtin(args, shell));
    return (exec_external(args, shell));
    return 0;
}
