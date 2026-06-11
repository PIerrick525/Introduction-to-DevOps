/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** specifier_int.c
*/

#include "my.h"

static long long get_int_arg(va_list *ap, char length)
{
    if (length == 'q')
        return va_arg(*ap, long long);
    if (length == 'l')
        return va_arg(*ap, long);
    if (length == 'h')
        return (short)va_arg(*ap, int);
    if (length == 'H')
        return (signed char)va_arg(*ap, int);
    return va_arg(*ap, int);
}

int specifier_int(va_list *ap, format_t *fmt)
{
    long long nb = get_int_arg(ap, fmt->length);

    return print_number_with_flags(fmt, nb, 'd');
}

int specifier_unsigned(va_list *ap, format_t *fmt)
{
    unsigned long long nb;

    if (fmt->length == 'q')
        nb = va_arg(*ap, unsigned long long);
    if (fmt->length == 'l')
        nb = va_arg(*ap, unsigned long);
    if (fmt->length == 'h')
        nb = (unsigned short)va_arg(*ap, unsigned int);
    if (fmt->length == 'H')
        nb = (unsigned char)va_arg(*ap, unsigned int);
    if (fmt->length != 'q' && fmt->length != 'l'
        && fmt->length != 'h' && fmt->length != 'H')
        nb = va_arg(*ap, unsigned int);
    return my_put_unsigned(nb);
}
