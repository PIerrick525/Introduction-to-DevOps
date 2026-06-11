/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** my_put_nbr.c
*/

#include "my.h"

int my_put_nbr(long long nb)
{
    int count = 0;

    if (nb < 0) {
        count += my_putchar('-');
        nb = -nb;
    }
    if (nb >= 10)
        count += my_put_nbr(nb / 10);
    count += my_putchar('0' + nb % 10);
    return count;
}

int my_put_unsigned(unsigned long long nb)
{
    int count = 0;

    if (nb >= 10)
        count += my_put_unsigned(nb / 10);
    count += my_putchar('0' + nb % 10);
    return count;
}
