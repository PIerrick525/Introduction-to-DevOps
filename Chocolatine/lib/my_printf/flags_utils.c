/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** Flags and padding utilities
*/

#include "my.h"

static int number_length(long long nb, int base_len)
{
    int len = 0;

    if (nb == 0)
        return 1;
    if (nb < 0)
        nb = -nb;
    while (nb > 0) {
        nb /= base_len;
        len++;
    }
    return len;
}

static int get_prefix_length(format_t *fmt, long long value, int is_hex)
{
    int prefix_len = 0;

    if (value < 0)
        return 1;
    if (fmt->flag_plus || fmt->flag_space)
        prefix_len = 1;
    if (fmt->flag_hash && value != 0 && is_hex)
        prefix_len = 2;
    return prefix_len;
}

static int print_hash_prefix(char specifier)
{
    if (specifier == 'x')
        return my_putstr("0x");
    if (specifier == 'X')
        return my_putstr("0X");
    if (specifier == 'o')
        return my_putchar('0');
    return 0;
}

static int print_prefix(format_t *fmt, long long value, char specifier)
{
    int count = 0;

    if (value < 0) {
        count += my_putchar('-');
        return count;
    }
    if (fmt->flag_plus)
        count += my_putchar('+');
    else if (fmt->flag_space)
        count += my_putchar(' ');
    if (fmt->flag_hash && value != 0)
        count += print_hash_prefix(specifier);
    return count;
}

static int get_base_length(char specifier)
{
    if (specifier == 'o')
        return 8;
    if (specifier == 'x' || specifier == 'X')
        return 16;
    return 10;
}

static int is_hex_specifier(char specifier)
{
    return (specifier == 'x' || specifier == 'X');
}

int apply(format_t *fmt, long long value, char specifier)
{
    int count = 0;
    int base_len = get_base_length(specifier);
    int num_len = number_length(value < 0 ? -value : value,
        base_len);
    int prefix_len = get_prefix_length(fmt, value,
        is_hex_specifier(specifier));
    int total_len = num_len + prefix_len;
    int padding = (fmt->width > total_len) ? fmt->width - total_len : 0;

    if (!fmt->flag_minus && !fmt->flag_zero && padding > 0)
        count += print_padding(padding, ' ');
    count += print_prefix(fmt, value, specifier);
    if (!fmt->flag_minus && fmt->flag_zero && padding > 0)
        count += print_padding(padding, '0');
    return count;
}

static int print_number_by_base(long long value, char specifier)
{
    int count = 0;

    if (specifier == 'd' || specifier == 'i')
        count += my_put_nbr(value < 0 ? -value : value);
    if (specifier == 'x')
        count += my_put_nbr_base(value < 0 ? -value : value,
            "0123456789abcdef");
    if (specifier == 'X')
        count += my_put_nbr_base(value < 0 ? -value : value,
            "0123456789ABCDEF");
    if (specifier == 'o')
        count += my_put_nbr_base(value, "01234567");
    return count;
}

int print_number_with_flags(format_t *fmt, long long value, char specifier)
{
    int count = 0;
    int base_len = get_base_length(specifier);
    int num_len = number_length(value < 0 ? -value : value,
        base_len);
    int prefix_len = get_prefix_length(fmt, value,
        is_hex_specifier(specifier));
    int total_len = num_len + prefix_len;
    int padding = (fmt->width > total_len) ? fmt->width - total_len : 0;

    count += apply(fmt, value, specifier);
    count += print_number_by_base(value, specifier);
    if (fmt->flag_minus && padding > 0)
        count += print_padding(padding, ' ');
    return count;
}
