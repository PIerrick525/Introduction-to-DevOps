/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** my_putstr.c
*/

#include "my.h"

int my_putstr(char const *str)
{
    int count = 0;

    if (!str)
        str = "(null)";
    while (str[count] != '\0') {
        my_putchar(str[count]);
        count++;
    }
    return count;
}

int print_nstr(char const *str, int n)
{
    int count = 0;

    while (count < n && str[count] != '\0') {
        my_putchar(str[count]);
        count++;
    }
    return count;
}
