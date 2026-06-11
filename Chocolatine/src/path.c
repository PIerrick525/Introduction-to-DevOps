/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** path
*/

#include "mysh.h"
#include <sys/stat.h>

static int is_exec(const char *path)
{
    if (!path)
        return 0;
    if (access(path, X_OK) == 0)
        return 1;
    return 0;
}

static char *search_in_dir(const char *dir, const char *cmd)
{
    char full[PATH_MAX];
    size_t dir_len;

    dir_len = strlen(dir);
    my_strcpy(full, dir);
    full[dir_len] = '/';
    my_strcpy(full + dir_len + 1, cmd);
    if (is_exec(full))
        return (my_strdup(full));
    return (NULL);
}

static char *search_in_path(const char *cmd, char *copy)
{
    char *dir;
    char *saveptr;
    char *result;

    dir = strtok_r(copy, ":", &saveptr);
    while (dir) {
        result = search_in_dir(dir, cmd);
        if (result)
            return (result);
        dir = strtok_r(NULL, ":", &saveptr);
    }
    return (NULL);
}

char *resolve_path(const char *cmd, env_t *env)
{
    char *path_env;
    char *copy;
    char *result;

    if (my_strchr(cmd, '/'))
        return (my_strdup(cmd));
    path_env = env_get(env, "PATH");
    if (!path_env)
        return (NULL);
    copy = my_strdup(path_env);
    if (!copy)
        return (NULL);
    result = search_in_path(cmd, copy);
    free(copy);
    return (result);
}
