/*
** EPITECH PROJECT, 2026
** G-CPE-210-STG-2-1-solostumper01-19
** File description:
** my_strdup
*/

#include "my.h"

char *my_strdup(const char *str)
{
    char *dup = NULL;
    int len = 0;
    int i = 0;

    if (str == NULL)
        return NULL;
    len = my_strlen(str);
    dup = malloc(sizeof(char) * (len + 1));
    if (dup == NULL)
        return NULL;
    for (i = 0; i < len; i++)
        dup[i] = str[i];
    dup[len] = '\0';
    return dup;
}
