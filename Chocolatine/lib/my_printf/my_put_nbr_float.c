/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** my_put_nbr_float.c
*/

#include "my.h"

static void print_dec(long long dec, int pr)
{
    char buf[50];
    int i = pr - 1;

    while (i >= 0) {
        buf[i] = (dec % 10) + '0';
        dec /= 10;
        i--;
    }
    for (i = 0; i < pr; i++)
        my_putchar(buf[i]);
}

int my_put_nbr_float(long double n, int pr)
{
    long long integer_part;
    long long decimals;
    int count = 0;

    if (n < 0) {
        count += my_putchar('-');
        n = -n;
    }
    integer_part = (long long)n;
    count += my_put_nbr(integer_part);
    count += my_putchar('.');
    decimals = (long long)((n - integer_part) * puissance(10, pr) + 0.5);
    print_dec(decimals, pr);
    return count + pr;
}
