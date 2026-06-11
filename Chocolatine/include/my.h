/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** Main header file
*/

#ifndef MY_H_
    #define MY_H_

    #include <stdarg.h>
    #include <stddef.h>
    #include <stdlib.h>

typedef struct s_format {
    int flag_hash;
    int flag_zero;
    int flag_minus;
    int flag_space;
    int flag_plus;
    int width;
    int precision;
    char length;
    char specifier;
    int *count_ptr;
} format_t;

typedef struct s_flag {
    char per;
    int (*func)(va_list *ap, format_t *fmt);
} flag_t;

void my_put_error(const char *str);
int my_strcmp(const char *str1, const char *str2);
int my_atoi(const char *str);
void *my_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char **my_split(const char *str, const char *separators);
const char *my_strchr_ptr(const char *s, int c);
void my_strcpy(char *dest, const char *src);
char *my_strdup(const char *str);
int my_strchr(const char *s, int c);
char **my_str_to_word_array(const char *str, char separator);
int my_printf(const char *format, ...);
int my_putchar(char c);
int my_putstr(char const *str);
int my_strlen(char const *str);
int my_put_nbr(long long nb);
void my_int_to_str(int nb, char *buf);
int my_put_unsigned(unsigned long long nb);
int my_put_nbr_base(unsigned long long nb, char const *base);
int my_put_ptr(void *ptr);
int my_put_nbr_float(long double n, int pr);
int my_put_nbr_exp(long double n, int pr, char e);
int my_put_nbr_auto(long double n, int pr, char e);
long long puissance(int base, int exp);
int print_padding(int count, char c);
int print_nstr(char const *str, int n);
void init_format(format_t *fmt);
void parse_flags(const char *fmt, int *i, format_t *f);
void parse_width(const char *fmt, int *i, format_t *f);
void parse_precision(const char *fmt, int *i, format_t *f);
void parse_length(const char *fmt, int *i, format_t *f);
int specifier_str(va_list *ap, format_t *fmt);
int specifier_binary(va_list *ap, format_t *fmt);
int specifier_char(va_list *ap, format_t *fmt);
int specifier_percent(va_list *ap, format_t *fmt);
int specifier_int(va_list *ap, format_t *fmt);
int specifier_unsigned(va_list *ap, format_t *fmt);
int specifier_hexa(va_list *ap, format_t *fmt);
int specifier_hexa_maj(va_list *ap, format_t *fmt);
int specifier_octal(va_list *ap, format_t *fmt);
int specifier_ptr(va_list *ap, format_t *fmt);
int specifier_float(va_list *ap, format_t *fmt);
int specifier_float_maj(va_list *ap, format_t *fmt);
int specifier_exp(va_list *ap, format_t *fmt);
int specifier_exp_maj(va_list *ap, format_t *fmt);
int specifier_auto(va_list *ap, format_t *fmt);
int specifier_auto_maj(va_list *ap, format_t *fmt);
int specifier_count(va_list *ap, format_t *fmt);
int apply_flags_and_padding(format_t *fmt, long long value, char specifier);
int print_number_with_flags(format_t *fmt, long long value, char specifier);
long double get_float(va_list *ap, char length);

#endif /* !MY_H_ */
