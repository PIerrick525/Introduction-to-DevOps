/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** my_put_ptr.c
*/

#include "my.h"

int my_put_ptr(void *ptr)
{
    unsigned long nb = (unsigned long)ptr;
    int count = 0;

    if (!ptr)
        return my_putstr("(nil)");
    count += my_putstr("0x");
    count += my_put_nbr_base(nb, "0123456789abcdef");
    return count;
}
