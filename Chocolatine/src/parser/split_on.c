/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** split_on
*/

#include "mysh.h"

static char update_quote(char quote, char c)
{
    if (!quote && (c == '\'' || c == '"'))
        return c;
    if (quote && c == quote)
        return 0;
    return quote;
}

static int count_segments(const char *line, char sep)
{
    int count = 1;
    char quote = 0;
    int i = 0;

    while (line[i]) {
        quote = update_quote(quote, line[i]);
        if (!quote && line[i] == sep)
            count++;
        i++;
    }
    return count;
}

static char *copy_trim(const char *line, int start, int len)
{
    while (len > 0 && (line[start] == ' ' || line[start] == '\t')) {
        start++;
        len--;
    }
    while (len > 0 && (line[start + len - 1] == ' '
            || line[start + len - 1] == '\t'))
        len--;
    return my_strndup(line + start, len);
}

static int fill_segment(char **res, const char *line, char sep, int j)
{
    char quote = 0;
    int seg_start = 0;
    int i = 0;

    while (line[i]) {
        quote = update_quote(quote, line[i]);
        if (!quote && line[i] == sep) {
            res[j] = copy_trim(line, seg_start, i - seg_start);
            j++;
            seg_start = i + 1;
        }
        i++;
    }
    res[j] = copy_trim(line, seg_start, i - seg_start);
    j++;
    res[j] = NULL;
    return j;
}

char **split_on(const char *line, char sep)
{
    int nb = count_segments(line, sep);
    char **result = malloc(sizeof(char *) * (nb + 1));

    if (!result || !line)
        return NULL;
    fill_segment(result, line, sep, 0);
    return result;
}
