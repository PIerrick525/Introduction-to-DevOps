/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** parser utils
*/

#include "mysh.h"

char *my_strndup(const char *str, int n)
{
    char *copy = malloc(sizeof(char) * (n + 1));
    int i = 0;

    if (!copy)
        return NULL;
    while (i < n && str[i]) {
        copy[i] = str[i];
        i++;
    }
    copy[i] = '\0';
    return copy;
}
