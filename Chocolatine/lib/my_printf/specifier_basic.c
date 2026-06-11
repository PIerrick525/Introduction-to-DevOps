/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** specifier_basic.c
*/

#include "my.h"

int specifier_str(va_list *ap, format_t *fmt)
{
    char *str = va_arg(*ap, char *);
    int len;
    int count = 0;

    if (!str)
        str = "(null)";
    len = my_strlen(str);
    if (fmt->precision >= 0 && fmt->precision < len)
        len = fmt->precision;
    if (!fmt->flag_minus && fmt->width > len)
        count += print_padding(fmt->width - len, ' ');
    count += print_nstr(str, len);
    if (fmt->flag_minus && fmt->width > len)
        count += print_padding(fmt->width - len, ' ');
    return count;
}

int specifier_char(va_list *ap, format_t *fmt)
{
    char c = (char)va_arg(*ap, int);
    int count = 0;

    if (!fmt->flag_minus && fmt->width > 1)
        count += print_padding(fmt->width - 1, ' ');
    count += my_putchar(c);
    if (fmt->flag_minus && fmt->width > 1)
        count += print_padding(fmt->width - 1, ' ');
    return count;
}

int specifier_percent(va_list *ap, format_t *fmt)
{
    (void)ap;
    (void)fmt;
    return my_putchar('%');
}

int specifier_count(va_list *ap, format_t *fmt)
{
    int *ptr = va_arg(*ap, int *);

    if (ptr && fmt->count_ptr)
        *ptr = *(fmt->count_ptr);
    return 0;
}
