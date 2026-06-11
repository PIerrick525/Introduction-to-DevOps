/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** my_realloc
*/

#include "my.h"

static unsigned int min(unsigned int a, unsigned int b)
{
    return (a < b ? a : b);
}

static void copy_bytes(void *dst, void *src, unsigned int size)
{
    unsigned char *d = (unsigned char *)dst;
    unsigned char *s = (unsigned char *)src;
    unsigned int i = 0;

    while (i < size) {
        d[i] = s[i];
        i++;
    }
}

void *my_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
    void *new_ptr;

    if (!ptr)
        return malloc(new_size);
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }
    new_ptr = malloc(new_size);
    if (!new_ptr)
        return NULL;
    copy_bytes(new_ptr, ptr, min(old_size, new_size));
    free(ptr);
    return new_ptr;
}
