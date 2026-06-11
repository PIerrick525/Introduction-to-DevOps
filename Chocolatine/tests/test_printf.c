/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** test_printf
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <string.h>

#include "my.h"

static void flush_capture(void)
{
    char capture[4096];

    memset(capture, 0, sizeof(capture));
}

Test(my_printf, simple_string, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("hello");
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "hello");
}

Test(my_printf, percent_d, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%d", 42);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "42");
}

Test(my_printf, percent_d_negative, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%d", -42);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "-42");
}

Test(my_printf, percent_s, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%s", "world");
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "world");
}

Test(my_printf, percent_s_null, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%s", NULL);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "(null)");
}

Test(my_printf, percent_c, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%c", 'A');
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_eq(buf[0], 'A');
}

Test(my_printf, percent_percent, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%%");
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "%");
}

Test(my_printf, percent_x, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%x", 255);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "ff");
}

Test(my_printf, percent_X, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%X", 255);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "FF");
}

Test(my_printf, percent_o, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%o", 8);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "10");
}

Test(my_printf, percent_u, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%u", 42u);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "42");
}

Test(my_printf, percent_b, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%b", 5u);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "101");
}

Test(my_printf, returns_char_count)
{
    int ret = my_printf("hello");

    cr_assert_eq(ret, 5);
}

Test(my_printf, returns_count_mixed)
{
    int ret = my_printf("%d %s", 42, "hi");

    cr_assert_eq(ret, 5);
}

Test(my_printf, width_right_align, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%5d", 42);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "   42");
}

Test(my_printf, width_left_align, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%-5d|", 42);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "42   |");
}

Test(my_printf, zero_padding, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%05d", 42);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "00042");
}

Test(my_printf, plus_flag_positive, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%+d", 42);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "+42");
}

Test(my_printf, plus_flag_negative, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%+d", -42);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "-42");
}

Test(my_printf, space_flag, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("% d", 42);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, " 42");
}

Test(my_printf, hash_flag_hex, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%#x", 255);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "0xff");
}

Test(my_printf, hash_flag_octal, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%#o", 8);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "010");
}

Test(my_printf, precision_string, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%.3s", "hello");
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "hel");
}

Test(my_printf, float_basic, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%f", 3.14);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert(strncmp(buf, "3.14", 4) == 0);
}

Test(my_printf, float_precision, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%.2f", 3.14159);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "3.14");
}

Test(my_printf, exp_format, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%e", 314.0);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert(strchr(buf, 'e') != NULL);
}

Test(my_printf, ptr_null, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%p", NULL);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "(nil)");
}

Test(my_printf, ptr_non_null, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;
    int x = 0;

    my_printf("%p", (void *)&x);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert(strncmp(buf, "0x", 2) == 0);
}

Test(my_printf, unknown_specifier, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_printf("%q");
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert(strncmp(buf, "%q", 2) == 0);
}

Test(my_putstr, basic, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_putstr("hello");
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "hello");
}

Test(my_putstr, null_prints_null_str, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    my_putstr(NULL);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "(null)");
}

Test(my_putstr, returns_length)
{
    int ret = my_putstr("hello");

    cr_assert_eq(ret, 5);
}

Test(print_nstr, basic, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    print_nstr("hello", 3);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "hel");
}

Test(print_nstr, full_string, .init = cr_redirect_stdout)
{
    char buf[256];
    FILE *out;
    int n;

    print_nstr("hello", 5);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "hello");
}

Test(my_strlen, basic)
{
    cr_assert_eq(my_strlen("hello"), 5);
}

Test(my_strlen, empty)
{
    cr_assert_eq(my_strlen(""), 0);
}

Test(my_strlen, single_char)
{
    cr_assert_eq(my_strlen("a"), 1);
}

Test(my_put_nbr, zero, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr(0);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "0");
}

Test(my_put_nbr, positive, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr(42);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "42");
}

Test(my_put_nbr, negative, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr(-42);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "-42");
}

Test(my_put_nbr_base, hex, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr_base(255, "0123456789abcdef");
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "ff");
}

Test(my_put_nbr_base, binary, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr_base(5, "01");
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "101");
}

Test(my_put_nbr_base, zero, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr_base(0, "0123456789abcdef");
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "0");
}

Test(my_put_ptr, null, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_ptr(NULL);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "(nil)");
}

Test(my_put_ptr, non_null, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;
    int x = 0;

    my_put_ptr((void *)&x);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert(strncmp(buf, "0x", 2) == 0);
}

Test(puissance, base_ten)
{
    cr_assert_eq(puissance(10, 3), 1000);
}

Test(puissance, base_two)
{
    cr_assert_eq(puissance(2, 8), 256);
}

Test(puissance, exp_zero)
{
    cr_assert_eq(puissance(10, 0), 1);
}

Test(puissance, base_zero)
{
    cr_assert_eq(puissance(0, 5), 0);
}

Test(my_strchr, found)
{
    cr_assert_eq(my_strchr("hello", 'e'), 1);
}

Test(my_strchr, not_found)
{
    cr_assert_eq(my_strchr("hello", 'z'), 0);
}

Test(my_strchr, first_char)
{
    cr_assert_eq(my_strchr("hello", 'h'), 1);
}

Test(my_strchr, empty_string)
{
    cr_assert_eq(my_strchr("", 'a'), 0);
}

Test(my_put_nbr_float, positive, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr_float(3.14, 2);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "3.14");
}

Test(my_put_nbr_float, negative, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr_float(-1.5, 1);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "-1.5");
}

Test(my_put_nbr_float, zero, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr_float(0.0, 2);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "0.00");
}

Test(my_put_nbr_exp, positive, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr_exp(314.0, 2, 'e');
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert(strchr(buf, 'e') != NULL);
}

Test(my_put_nbr_exp, negative, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr_exp(-1.0, 2, 'e');
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert(buf[0] == '-');
}

Test(my_put_nbr_exp, zero, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_nbr_exp(0.0, 2, 'e');
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert(strchr(buf, 'e') != NULL);
}

Test(my_put_unsigned, basic, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_unsigned(42);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "42");
}

Test(my_put_unsigned, zero, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    my_put_unsigned(0);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "0");
}

Test(print_padding, spaces, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;
    int ret;

    ret = print_padding(3, ' ');
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "   ");
    cr_assert_eq(ret, 3);
}

Test(print_padding, zeros, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    int n;

    print_padding(4, '0');
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[n] = '\0';
    cr_assert_str_eq(buf, "0000");
}
