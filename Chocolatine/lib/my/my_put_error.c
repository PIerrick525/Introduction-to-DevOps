/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** my_put_error
*/

#include <unistd.h>

void my_put_error(const char *str)
{
    int i = 0;

    if (!str)
        return;
    while (str[i])
        i++;
    write(STDERR_FILENO, str, i);
}
