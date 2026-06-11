/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** my_strchr_ptr
*/

#include "my.h"

const char *my_strchr_ptr(const char *s, int c)
{
    while (*s) {
        if (*s == (char)c)
            return s;
        s++;
    }
    return NULL;
}
