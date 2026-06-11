/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** setenv
*/
#include "mysh.h"

static int is_valid_name(const char *name)
{
    int i = 1;

    while (name[i]) {
        if (!((name[i] >= 'a' && name[i] <= 'z') ||
                (name[i] >= 'A' && name[i] <= 'Z') ||
                (name[i] >= '0' && name[i] <= '9') ||
                name[i] == '_'))
            return 0;
        i++;
    }
    return 1;
}

static int is_valid_start(const char *name)
{
    int i = 0;

    if (!name || name[0] == '\0')
        return 0;
    if (!((name[0] >= 'a' && name[0] <= 'z') ||
            (name[0] >= 'A' && name[0] <= 'Z') ||
            name[0] == '_'))
        return 0;
    return 1;
}

static int validate_setenv_name(const char *name)
{
    if (!is_valid_start(name)) {
        my_put_error("setenv: Variable name must ");
        my_put_error("begin with a letter.\n");
        return 84;
    }
    if (!is_valid_name(name)) {
        my_put_error("setenv: Variable name must contain");
        my_put_error(" alphanumeric characters.\n");
        return 84;
    }
    return 0;
}

static int apply_setenv(shell_t *shell, const char *name, char **args)
{
    const char *value = args[2] ? args[2] : "";

    if (env_set(&shell->env, name, value) != 0) {
        my_put_error("setenv: Cannot set variable.\n");
        return 84;
    }
    return 0;
}

int builtin_setenv(char **args, shell_t *shell)
{
    if (!args[1]) {
        builtin_env(args, shell);
        return 0;
    }
    if (validate_setenv_name(args[1]) != 0)
        return 84;
    return apply_setenv(shell, args[1], args);
}
