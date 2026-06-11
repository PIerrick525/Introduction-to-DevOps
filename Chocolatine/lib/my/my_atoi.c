/*
** EPITECH PROJECT, 2026
** G-CPE-210-STG-2-1-solostumper01-19
** File description:
** my_atoi
*/

#include "my.h"

int my_atoi(const char *str)
{
    int result = 0;
    int i = 0;
    int sign = 1;

    if (str == NULL)
        return 0;
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result * sign;
}
