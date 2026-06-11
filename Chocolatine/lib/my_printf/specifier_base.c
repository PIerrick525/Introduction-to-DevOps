/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** specifier_base.c
*/

#include "my.h"

int specifier_hexa(va_list *ap, format_t *fmt)
{
    unsigned int nb = va_arg(*ap, unsigned int);

    return print_number_with_flags(fmt, nb, 'x');
}

int specifier_hexa_maj(va_list *ap, format_t *fmt)
{
    unsigned int nb = va_arg(*ap, unsigned int);

    return print_number_with_flags(fmt, nb, 'X');
}

int specifier_octal(va_list *ap, format_t *fmt)
{
    unsigned int nb = va_arg(*ap, unsigned int);

    return print_number_with_flags(fmt, nb, 'o');
}

int specifier_ptr(va_list *ap, format_t *fmt)
{
    void *ptr = va_arg(*ap, void *);

    (void)fmt;
    return my_put_ptr(ptr);
}
