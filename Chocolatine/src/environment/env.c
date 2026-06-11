/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** env
*/

#include "mysh.h"

static void add_node(env_t **head, env_t **tail, env_t *node)
{
    if (!*head)
        *head = node;
    else
        (*tail)->next = node;
    *tail = node;
}

env_t *env_copy(char **envp)
{
    env_t *head;
    env_t *tail;
    env_t *node;
    int i;

    if (!envp)
        return (NULL);
    head = NULL;
    tail = NULL;
    i = 0;
    while (envp[i]) {
        node = env_new_node(envp[i]);
        if (!node) {
            env_free(head);
            return (NULL);
        }
        add_node(&head, &tail, node);
        i++;
    }
    return (head);
}

char *env_get(env_t *env, const char *key)
{
    while (env) {
        if (my_strcmp(env->key, key) == 0)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}
