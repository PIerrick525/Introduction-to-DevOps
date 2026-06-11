/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** my_printf.c
*/

#include "my.h"

static const flag_t g_specifiers[] = {
    {'d', specifier_int}, {'i', specifier_int},
    {'s', specifier_str}, {'c', specifier_char},
    {'u', specifier_unsigned}, {'x', specifier_hexa},
    {'X', specifier_hexa_maj}, {'o', specifier_octal},
    {'p', specifier_ptr}, {'%', specifier_percent},
    {'f', specifier_float}, {'F', specifier_float_maj},
    {'e', specifier_exp}, {'E', specifier_exp_maj},
    {'g', specifier_auto}, {'G', specifier_auto_maj},
    {'n', specifier_count}, {'b', specifier_binary},
    {'\0', NULL}
};

static int handle_spec(va_list *ap, format_t *fmt)
{
    int k = 0;

    while (g_specifiers[k].per != '\0') {
        if (fmt->specifier == g_specifiers[k].per)
            return g_specifiers[k].func(ap, fmt);
        k++;
    }
    my_putchar('%');
    my_putchar(fmt->specifier);
    return 2;
}

static int process_format(const char *fmt, int *i, va_list *ap, int *cnt)
{
    format_t f;

    init_format(&f);
    parse_flags(fmt, i, &f);
    parse_width(fmt, i, &f);
    parse_precision(fmt, i, &f);
    parse_length(fmt, i, &f);
    f.specifier = fmt[*i];
    f.count_ptr = cnt;
    return handle_spec(ap, &f);
}

int my_printf(const char *format, ...)
{
    va_list ap;
    int i = 0;
    int count = 0;

    va_start(ap, format);
    while (format[i] != '\0') {
        if (format[i] == '%') {
            i++;
            count += process_format(format, &i, &ap, &count);
        } else {
            my_putchar(format[i]);
            count++;
        }
        i++;
    }
    va_end(ap);
    return count;
}
