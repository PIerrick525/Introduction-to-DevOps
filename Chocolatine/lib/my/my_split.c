/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** my_split
*/

#include "my.h"

static int is_separator(char c, const char *separators)
{
    int i = 0;

    while (separators[i] != '\0') {
        if (c == separators[i])
            return 1;
        i++;
    }
    return 0;
}

static char *replace_separators(const char *str, const char *separators)
{
    int len = my_strlen(str);
    char *copy = malloc(sizeof(char) * (len + 1));
    int i = 0;

    if (!copy)
        return NULL;
    while (i <= len) {
        copy[i] = is_separator(str[i], separators) ? ' ' : str[i];
        i++;
    }
    return copy;
}

char **my_split(const char *str, const char *separators)
{
    char *copy;
    char **result;

    if (!str || !separators)
        return NULL;
    copy = replace_separators(str, separators);
    if (!copy)
        return NULL;
    result = my_str_to_word_array(copy, ' ');
    free(copy);
    return result;
}
