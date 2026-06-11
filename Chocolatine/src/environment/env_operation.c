/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** env operations
*/
#include "mysh.h"

static int update_existing_value(env_t *cur, const char *value)
{
    free(cur->value);
    cur->value = my_strdup(value);
    if (!cur->value)
        return (84);
    return (0);
}

static env_t *find_env_key(env_t *env, const char *key)
{
    while (env) {
        if (my_strcmp(env->key, key) == 0)
            return (env);
        env = env->next;
    }
    return (NULL);
}

static env_t *create_env_node(const char *key, const char *value)
{
    env_t *node;

    node = malloc(sizeof(env_t));
    if (!node)
        return (NULL);
    node->key = my_strdup(key);
    node->value = my_strdup(value);
    node->next = NULL;
    if (!node->key || !node->value) {
        free(node->key);
        free(node->value);
        free(node);
        return (NULL);
    }
    return (node);
}

static void append_env_node(env_t **env, env_t *node)
{
    env_t *cur;

    if (!*env) {
        *env = node;
        return;
    }
    cur = *env;
    while (cur->next)
        cur = cur->next;
    cur->next = node;
}

int env_set(env_t **env, const char *key, const char *value)
{
    env_t *cur;
    env_t *node;

    cur = find_env_key(*env, key);
    if (cur)
        return (update_existing_value(cur, value));
    node = create_env_node(key, value);
    if (!node)
        return 84;
    append_env_node(env, node);
    return 0;
}

static void remove_env_node(env_t **env, env_t *cur, env_t *prev)
{
    if (prev)
        prev->next = cur->next;
    else
        *env = cur->next;
    free(cur->key);
    free(cur->value);
    free(cur);
}

void env_unset(env_t **env, const char *key)
{
    env_t *cur;
    env_t *prev;

    cur = *env;
    prev = NULL;
    while (cur) {
        if (my_strcmp(cur->key, key) == 0) {
            remove_env_node(env, cur, prev);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}
