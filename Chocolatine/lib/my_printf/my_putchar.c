/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** my_putchar.c
*/

#include <unistd.h>

int my_putchar(char c)
{
    write(1, &c, 1);
    return 1;
}
