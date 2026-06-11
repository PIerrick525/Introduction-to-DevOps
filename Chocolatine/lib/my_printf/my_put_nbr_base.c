/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** my_put_nbr_base.c
*/

#include "my.h"

int my_put_nbr_base(unsigned long long nb, char const *base)
{
    unsigned long long base_len = 0;
    int count = 0;

    while (base[base_len] != '\0')
        base_len++;
    if (nb >= base_len)
        count += my_put_nbr_base(nb / base_len, base);
    count += my_putchar(base[nb % base_len]);
    return count;
}
