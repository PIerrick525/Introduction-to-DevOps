/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** specifier_float.c
*/

#include "my.h"

long double get_float(va_list *ap, char length)
{
    if (length == 'L')
        return va_arg(*ap, long double);
    return va_arg(*ap, double);
}

int specifier_float(va_list *ap, format_t *fmt)
{
    long double nb = get_float(ap, fmt->length);
    int pr = (fmt->precision < 0) ? 6 : fmt->precision;

    return my_put_nbr_float(nb, pr);
}

int specifier_float_maj(va_list *ap, format_t *fmt)
{
    return specifier_float(ap, fmt);
}

int specifier_exp(va_list *ap, format_t *fmt)
{
    long double nb = get_float(ap, fmt->length);
    int pr = (fmt->precision < 0) ? 6 : fmt->precision;

    return my_put_nbr_exp(nb, pr, 'e');
}

int specifier_exp_maj(va_list *ap, format_t *fmt)
{
    long double nb = get_float(ap, fmt->length);
    int pr = (fmt->precision < 0) ? 6 : fmt->precision;

    return my_put_nbr_exp(nb, pr, 'E');
}
