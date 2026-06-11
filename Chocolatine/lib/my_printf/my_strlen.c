/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** my_strlen.c
*/

int my_strlen(char const *str)
{
    int count = 0;

    while (str[count] != '\0')
        count++;
    return count;
}
