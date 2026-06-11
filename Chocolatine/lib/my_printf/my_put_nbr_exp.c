/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** my_put_nbr_exp.c
*/

#include "my.h"

static void get_exponent(long double *n, int *exp)
{
    while (*n >= 10.0) {
        *n /= 10.0;
        (*exp)++;
    }
    while (*n < 1.0 && *n != 0.0) {
        *n *= 10.0;
        (*exp)--;
    }
}

static int print_exp(int exp, char e_char)
{
    int count = 0;

    count += my_putchar(e_char);
    if (exp < 0) {
        count += my_putchar('-');
        exp = -exp;
    } else
        count += my_putchar('+');
    if (exp < 10)
        count += my_putchar('0');
    count += my_put_nbr(exp);
    return count;
}

static int print_decimals(long double n, int pr)
{
    int count = 0;
    int digit;

    for (int i = 0; i < pr; i++) {
        n = n - (int)n;
        n *= 10.0;
        digit = (int)(n + 0.0000001);
        count += my_putchar('0' + digit);
    }
    return count;
}

int my_put_nbr_exp(long double n, int pr, char e_char)
{
    int exp = 0;
    int count = 0;

    if (n < 0) {
        count += my_putchar('-');
        n = -n;
    }
    if (n == 0.0)
        exp = 0;
    else
        get_exponent(&n, &exp);
    count += my_put_nbr((int)n);
    count += my_putchar('.');
    count += print_decimals(n, pr);
    count += print_exp(exp, e_char);
    return count;
}
