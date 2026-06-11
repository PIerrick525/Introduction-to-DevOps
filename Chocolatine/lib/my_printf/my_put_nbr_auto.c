/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** my_put_nbr_auto.c
*/

#include "my.h"

static int get_exp(long double n)
{
    int exp = 0;

    if (n == 0.0)
        return 0;
    if (n < 0)
        n = -n;
    while (n >= 10.0) {
        n /= 10.0;
        exp++;
    }
    while (n < 1.0) {
        n *= 10.0;
        exp--;
    }
    return exp;
}

int my_put_nbr_auto(long double n, int pr, char e_char)
{
    int exp;

    if (pr == 0)
        pr = 1;
    exp = get_exp(n);
    if (exp < -4 || exp >= pr)
        return my_put_nbr_exp(n, pr - 1, e_char);
    return my_put_nbr_float(n, pr - exp - 1);
}
