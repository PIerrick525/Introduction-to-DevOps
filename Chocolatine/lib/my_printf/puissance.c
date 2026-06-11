/*
** EPITECH PROJECT, 2025
** my_printf
** File description:
** puissance.c
*/

long long puissance(int base, int exp)
{
    long long res = 1;

    for (int i = 0; i < exp; i++)
        res *= base;
    return res;
}
