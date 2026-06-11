/*
** EPITECH PROJECT, 2025
** G-PSU-100-STG-1-1-myls-10
** File description:
** utils
*/

#include "my.h"

int my_strcmp(const char *str1, const char *str2)
{
    int i = 0;

    if (!str1 || !str2)
        return 0;
    while (str1[i] && str2[i] && str1[i] == str2[i])
        i++;
    return str1[i] - str2[i];
}
