/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** specifier_float.c
*/

#include "my.h"

int specifier_auto(va_list *ap, format_t *fmt)
{
    long double nb = get_float(ap, fmt->length);
    int pr = (fmt->precision < 0) ? 6 : fmt->precision;

    return my_put_nbr_auto(nb, pr, 'e');
}

int specifier_auto_maj(va_list *ap, format_t *fmt)
{
    long double nb = get_float(ap, fmt->length);
    int pr = (fmt->precision < 0) ? 6 : fmt->precision;

    return my_put_nbr_auto(nb, pr, 'E');
}

int specifier_binary(va_list *ap, format_t *fmt)
{
    unsigned int nb = va_arg(*ap, unsigned int);

    (void)fmt;
    return my_put_nbr_base(nb, "01");
}
