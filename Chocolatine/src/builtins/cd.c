/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** cd
*/
#include "mysh.h"

static char *get_home_dir(shell_t *shell)
{
    char *home;

    home = env_get(shell->env, "HOME");
    if (!home) {
        my_put_error("cd: No home directory.\n");
        return NULL;
    }
    return my_strdup(home);
}

static char *get_oldpwd_dir(shell_t *shell)
{
    char *oldpwd;

    oldpwd = env_get(shell->env, "OLDPWD");
    if (!oldpwd || oldpwd[0] == '\0') {
        my_put_error("cd: OLDPWD not set.\n");
        return NULL;
    }
    my_printf("%s\n", oldpwd);
    return my_strdup(oldpwd);
}

static char *get_target_dir(char **args, shell_t *shell, int *need_free)
{
    if (!args[1] || my_strcmp(args[1], "~") == 0) {
        *need_free = 1;
        return get_home_dir(shell);
    }
    if (my_strcmp(args[1], "-") == 0) {
        *need_free = 1;
        return get_oldpwd_dir(shell);
    }
    *need_free = 0;
    return args[1];
}

static int change_directory(char *target)
{
    if (chdir(target) == -1) {
        my_put_error("cd: ");
        my_put_error(target);
        my_put_error(": ");
        my_put_error(strerror(errno));
        my_put_error(".\n");
        return 84;
    }
    return 0;
}

static void save_oldpwd(shell_t *shell)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)))
        env_set(&shell->env, "OLDPWD", cwd);
}

static void update_pwd(shell_t *shell)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)))
        env_set(&shell->env, "PWD", cwd);
}

static int many_argument(char **args)
{
    if (args[1] && args[2]) {
        my_put_error("cd: Too many arguments.\n");
        return 84;
    }
    return 0;
}

int builtin_cd(char **args, shell_t *shell)
{
    char *target;
    int need_free;
    int ret;

    if (many_argument(args) == 84)
        return 84;
    target = get_target_dir(args, shell, &need_free);
    if (!target)
        return 84;
    save_oldpwd(shell);
    ret = change_directory(target);
    if (ret == 84) {
        if (need_free)
            free(target);
        return 84;
    }
    update_pwd(shell);
    if (need_free)
        free(target);
    return 0;
}
