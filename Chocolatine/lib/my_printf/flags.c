/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** flag.c
*/

#include "my.h"

int print_padding(int count, char c)
{
    int i = 0;

    while (i < count) {
        my_putchar(c);
        i++;
    }
    return count;
}
