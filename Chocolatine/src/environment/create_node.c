/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** create_node
*/

#include "mysh.h"

static env_t *init_env_node(void)
{
    env_t *node;

    node = malloc(sizeof(env_t));
    if (!node)
        return (NULL);
    node->key = NULL;
    node->value = NULL;
    node->next = NULL;
    return (node);
}

static char *get_value(const char *entry)
{
    const char *eq = my_strchr_ptr(entry, '=');

    if (!eq)
        return my_strdup("");
    return my_strdup(eq + 1);
}

static char *get_key(const char *entry)
{
    const char *eq = my_strchr_ptr(entry, '=');
    int len;

    if (!eq)
        return my_strdup(entry);
    len = eq - entry;
    return my_strndup(entry, len);
}

static int fill_env_node(env_t *node, const char *entry)
{
    node->key = get_key(entry);
    node->value = get_value(entry);
    if (!node->key || !node->value)
        return (0);
    return (1);
}

env_t *env_new_node(const char *entry)
{
    env_t *node;

    if (!entry || entry[0] == '\0')
        return (NULL);
    node = init_env_node();
    if (!node)
        return (NULL);
    if (!fill_env_node(node, entry)) {
        free(node->key);
        free(node->value);
        free(node);
        return (NULL);
    }
    return (node);
}
