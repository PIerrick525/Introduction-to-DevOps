/*
** EPITECH PROJECT, 2025
** G-PSU-100-STG-1-1-mysudo-16
** File description:
** my_str_to_word_array
*/

#include <stdlib.h>

static int is_char_of_word(char c, char separator)
{
    return !(c == separator || c == '\t' || c == '\n');
}

static int word_number(char const *str, char separator)
{
    int count = 0;
    int in_word = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (is_char_of_word(str[i], separator) && !in_word) {
            in_word = 1;
            count++;
        }
        if (!is_char_of_word(str[i], separator)) {
            in_word = 0;
        }
    }
    return count;
}

static int word_len(char const *str, int start, char separator)
{
    int len = 0;

    while (str[start + len] != '\0' &&
        is_char_of_word(str[start + len], separator)) {
        len++;
    }
    return len;
}

static char *copy_word(char const *str, int start, int len)
{
    char *word = malloc(sizeof(char) * (len + 1));

    if (!word)
        return NULL;
    for (int k = 0; k < len; k++) {
        word[k] = str[start + k];
    }
    word[len] = '\0';
    return word;
}

static void populate_array(char **tab, const char *str, char separator)
{
    int i = 0;
    int j = 0;
    int len;

    while (str[i] != '\0') {
        if (is_char_of_word(str[i], separator)) {
            len = word_len(str, i, separator);
            tab[j] = copy_word(str, i, len);
            j++;
            i += len;
        } else {
            i++;
        }
    }
    tab[j] = NULL;
}

char **my_str_to_word_array(const char *str, char separator)
{
    int nb_words = word_number(str, separator);
    char **tab = malloc(sizeof(char *) * (nb_words + 1));

    if (!tab || !str)
        return NULL;
    populate_array(tab, str, separator);
    return tab;
}
