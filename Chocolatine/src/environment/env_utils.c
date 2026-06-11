/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** env_utils
*/

#include "mysh.h"

static int count_nodes(env_t *env)
{
    int count;

    count = 0;
    while (env) {
        count++;
        env = env->next;
    }
    return (count);
}

static char *create_env_string(env_t *node)
{
    size_t len;
    char *str;
    char *ptr;

    len = strlen(node->key) + strlen(node->value) + 2;
    str = malloc(len);
    if (!str)
        return (NULL);
    ptr = str;
    my_strcpy(ptr, node->key);
    ptr += strlen(node->key);
    *ptr = '=';
    ptr++;
    my_strcpy(ptr, node->value);
    return (str);
}

static int fill_env_array(char **arr, env_t *env)
{
    int i;

    i = 0;
    while (env) {
        arr[i] = create_env_string(env);
        if (!arr[i]) {
            env_free_array(arr);
            return (-1);
        }
        i++;
        env = env->next;
    }
    arr[i] = NULL;
    return (0);
}

char **env_to_array(env_t *env)
{
    char **arr;
    int count;

    count = count_nodes(env);
    arr = malloc(sizeof(char *) * (count + 1));
    if (!arr)
        return (NULL);
    if (fill_env_array(arr, env) == -1)
        return (NULL);
    return (arr);
}

void env_free(env_t *env)
{
    env_t *next;

    while (env) {
        next = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = next;
    }
}

void env_free_array(char **arr)
{
    int i;

    if (!arr)
        return;
    i = 0;
    while (arr[i]) {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void free_tokens(char **tokens)
{
    int i;

    if (!tokens)
        return;
    i = 0;
    while (tokens[i]) {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}
