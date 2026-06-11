/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** my_strchr
*/

#include "my.h"

int my_strchr(const char *s, int c)
{
    while (*s) {
        if (*s == (char)c)
            return (1);
        s++;
    }
    return (0);
}
