/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** parser.c
*/

#include "my.h"

void init_format(format_t *fmt)
{
    fmt->flag_hash = 0;
    fmt->flag_zero = 0;
    fmt->flag_minus = 0;
    fmt->flag_space = 0;
    fmt->flag_plus = 0;
    fmt->width = 0;
    fmt->precision = -1;
    fmt->length = '\0';
    fmt->specifier = '\0';
    fmt->count_ptr = NULL;
}

void parse_flags(const char *format, int *i, format_t *fmt)
{
    while (format[*i] == '#' || format[*i] == '0' ||
        format[*i] == '-' || format[*i] == ' ' ||
        format[*i] == '+') {
        if (format[*i] == '#')
            fmt->flag_hash = 1;
        if (format[*i] == '0')
            fmt->flag_zero = 1;
        if (format[*i] == '-')
            fmt->flag_minus = 1;
        if (format[*i] == ' ')
            fmt->flag_space = 1;
        if (format[*i] == '+')
            fmt->flag_plus = 1;
        (*i)++;
    }
}

void parse_width(const char *format, int *i, format_t *fmt)
{
    while (format[*i] >= '0' && format[*i] <= '9') {
        fmt->width = fmt->width * 10 + (format[*i] - '0');
        (*i)++;
    }
}

void parse_precision(const char *format, int *i, format_t *fmt)
{
    if (format[*i] != '.')
        return;
    fmt->precision = 0;
    (*i)++;
    while (format[*i] >= '0' && format[*i] <= '9') {
        fmt->precision = fmt->precision * 10 + (format[*i] - '0');
        (*i)++;
    }
}

void parse_length(const char *format, int *i, format_t *fmt)
{
    if (format[*i] == 'l' && format[*i + 1] == 'l') {
        fmt->length = 'q';
        (*i) += 2;
        return;
    }
    if (format[*i] == 'h' && format[*i + 1] == 'h') {
        fmt->length = 'H';
        (*i) += 2;
        return;
    }
    if (format[*i] == 'l' || format[*i] == 'h' ||
        format[*i] == 'L' || format[*i] == 'j' ||
        format[*i] == 'z' || format[*i] == 't') {
        fmt->length = format[*i];
        (*i)++;
    }
}
