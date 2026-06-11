/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** test_lib
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "my.h"

Test(my_atoi, zero)
{
    cr_assert_eq(my_atoi("0"), 0);
}

Test(my_atoi, positive_simple)
{
    cr_assert_eq(my_atoi("42"), 42);
}

Test(my_atoi, negative_simple)
{
    cr_assert_eq(my_atoi("-42"), -42);
}

Test(my_atoi, large_positive)
{
    cr_assert_eq(my_atoi("1000000"), 1000000);
}

Test(my_atoi, large_negative)
{
    cr_assert_eq(my_atoi("-1000000"), -1000000);
}

Test(my_atoi, null_returns_zero)
{
    cr_assert_eq(my_atoi(NULL), 0);
}

Test(my_atoi, empty_string)
{
    cr_assert_eq(my_atoi(""), 0);
}

Test(my_atoi, stops_at_non_digit)
{
    cr_assert_eq(my_atoi("12abc"), 12);
}

Test(my_atoi, leading_minus_only)
{
    cr_assert_eq(my_atoi("-"), 0);
}

Test(my_atoi, single_digit)
{
    cr_assert_eq(my_atoi("7"), 7);
}

Test(my_atoi, single_negative_digit)
{
    cr_assert_eq(my_atoi("-3"), -3);
}

Test(my_int_to_str, zero)
{
    char buf[64];

    my_int_to_str(0, buf);
    cr_assert_str_eq(buf, "0");
}

Test(my_int_to_str, positive)
{
    char buf[64];

    my_int_to_str(42, buf);
    cr_assert_str_eq(buf, "42");
}

Test(my_int_to_str, negative)
{
    char buf[64];

    my_int_to_str(-42, buf);
    cr_assert_str_eq(buf, "-42");
}

Test(my_int_to_str, large_positive)
{
    char buf[64];

    my_int_to_str(1000000, buf);
    cr_assert_str_eq(buf, "1000000");
}

Test(my_int_to_str, large_negative)
{
    char buf[64];

    my_int_to_str(-1000000, buf);
    cr_assert_str_eq(buf, "-1000000");
}

Test(my_int_to_str, single_digit)
{
    char buf[64];

    my_int_to_str(9, buf);
    cr_assert_str_eq(buf, "9");
}

Test(my_int_to_str, roundtrip_with_atoi)
{
    char buf[64];

    my_int_to_str(12345, buf);
    cr_assert_eq(my_atoi(buf), 12345);
}

Test(my_put_error, basic_message, .init = cr_redirect_stderr)
{
    char buf[256];
    FILE *err;
    ssize_t n;

    my_put_error("error message\n");
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert_str_eq(buf, "error message\n");
}

Test(my_put_error, null_no_crash)
{
    my_put_error(NULL);
    cr_assert(1, "my_put_error(NULL) must not crash");
}

Test(my_put_error, empty_string, .init = cr_redirect_stderr)
{
    char buf[64];
    FILE *err;
    ssize_t n;

    my_put_error("");
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert_eq(n, 0, "my_put_error(\"\") must write nothing");
}

Test(my_put_error, goes_to_stderr_not_stdout, .init = cr_redirect_stdout)
{
    char buf[64];
    FILE *out;
    ssize_t n;

    my_put_error("should not appear on stdout\n");
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    cr_assert_eq(n, 0, "my_put_error must not write to stdout");
}

Test(my_realloc, null_ptr_acts_like_malloc)
{
    int *p = my_realloc(NULL, 0, sizeof(int) * 4);

    cr_assert_not_null(p);
    free(p);
}

Test(my_realloc, new_size_zero_frees_and_returns_null)
{
    int *p = malloc(sizeof(int) * 4);

    p = my_realloc(p, sizeof(int) * 4, 0);
    cr_assert_null(p);
}

Test(my_realloc, data_preserved_on_shrink)
{
    char *p = malloc(8);

    memcpy(p, "hello", 6);
    p = my_realloc(p, 8, 3);
    cr_assert_not_null(p);
    cr_assert_eq(p[0], 'h');
    cr_assert_eq(p[1], 'e');
    cr_assert_eq(p[2], 'l');
    free(p);
}

Test(my_realloc, data_preserved_on_grow)
{
    char *p = malloc(4);

    memcpy(p, "abc", 4);
    p = my_realloc(p, 4, 16);
    cr_assert_not_null(p);
    cr_assert_str_eq(p, "abc");
    free(p);
}

Test(my_realloc, same_size)
{
    char *p = malloc(8);

    memcpy(p, "test", 5);
    p = my_realloc(p, 8, 8);
    cr_assert_not_null(p);
    cr_assert_str_eq(p, "test");
    free(p);
}

Test(my_realloc, grow_then_shrink_chain)
{
    char *p = malloc(4);

    memcpy(p, "abc", 4);
    p = my_realloc(p, 4, 64);
    cr_assert_not_null(p);
    cr_assert_str_eq(p, "abc");
    p = my_realloc(p, 64, 4);
    cr_assert_not_null(p);
    free(p);
}

Test(my_strchr_ptr, finds_first_occurrence)
{
    const char *s = "hello";
    const char *p = my_strchr_ptr(s, 'l');

    cr_assert_not_null(p);
    cr_assert_eq(p - s, 2);
}

Test(my_strchr_ptr, char_not_present)
{
    const char *p = my_strchr_ptr("hello", 'z');

    cr_assert_null(p);
}

Test(my_strchr_ptr, first_char)
{
    const char *s = "hello";
    const char *p = my_strchr_ptr(s, 'h');

    cr_assert_not_null(p);
    cr_assert_eq(p, s);
}

Test(my_strchr_ptr, last_char)
{
    const char *s = "hello";
    const char *p = my_strchr_ptr(s, 'o');

    cr_assert_not_null(p);
    cr_assert_eq(p - s, 4);
}

Test(my_strchr_ptr, empty_string)
{
    cr_assert_null(my_strchr_ptr("", 'a'));
}

Test(my_strcmp, equal_strings)
{
    cr_assert_eq(my_strcmp("hello", "hello"), 0);
}

Test(my_strcmp, first_less_than_second)
{
    cr_assert_lt(my_strcmp("abc", "abd"), 0);
}

Test(my_strcmp, first_greater_than_second)
{
    cr_assert_gt(my_strcmp("abd", "abc"), 0);
}

Test(my_strcmp, empty_strings)
{
    cr_assert_eq(my_strcmp("", ""), 0);
}

Test(my_strcmp, prefix_shorter)
{
    cr_assert_lt(my_strcmp("abc", "abcd"), 0);
}

Test(my_strcmp, prefix_longer)
{
    cr_assert_gt(my_strcmp("abcd", "abc"), 0);
}

Test(my_strcmp, null_first)
{
    cr_assert_eq(my_strcmp(NULL, "abc"), 0);
}

Test(my_strcmp, null_second)
{
    cr_assert_eq(my_strcmp("abc", NULL), 0);
}

Test(my_strcmp, both_null)
{
    cr_assert_eq(my_strcmp(NULL, NULL), 0);
}

Test(my_strcpy, basic_copy)
{
    char dest[32];

    my_strcpy(dest, "hello");
    cr_assert_str_eq(dest, "hello");
}

Test(my_strcpy, empty_string)
{
    char dest[32];

    dest[0] = 'x';
    my_strcpy(dest, "");
    cr_assert_str_eq(dest, "");
}

Test(my_strcpy, null_terminator_written)
{
    char dest[8];

    memset(dest, 0xFF, sizeof(dest));
    my_strcpy(dest, "hi");
    cr_assert_eq(dest[2], '\0');
}

Test(my_strcpy, long_string)
{
    char dest[128];

    my_strcpy(dest, "aaaaabbbbbcccccddddddeeeeerrrrrfffffeeeezzzza");
    cr_assert_str_eq(dest, "aaaaabbbbbcccccddddddeeeeerrrrrfffffeeeezzzza");
}

Test(my_strdup, basic_dup)
{
    char *d = my_strdup("hello");

    cr_assert_not_null(d);
    cr_assert_str_eq(d, "hello");
    free(d);
}

Test(my_strdup, empty_string)
{
    char *d = my_strdup("");

    cr_assert_not_null(d);
    cr_assert_str_eq(d, "");
    free(d);
}

Test(my_strdup, null_returns_null)
{
    cr_assert_null(my_strdup(NULL));
}

Test(my_strdup, independent_copy)
{
    char src[] = "hello";
    char *d = my_strdup(src);

    cr_assert_not_null(d);
    src[0] = 'X';
    cr_assert_str_eq(d, "hello",
        "strdup must return an independent copy");
    free(d);
}

Test(my_strdup, correct_length)
{
    char *d = my_strdup("abcde");

    cr_assert_not_null(d);
    cr_assert_eq(strlen(d), 5u);
    free(d);
}

Test(my_strdup, long_string_allocation)
{
    char src[1024];
    char *d;

    memset(src, 'A', 1023);
    src[1023] = '\0';
    d = my_strdup(src);
    cr_assert_not_null(d);
    cr_assert_eq(strlen(d), 1023u);
    cr_assert_eq(d[0], 'A');
    cr_assert_eq(d[1022], 'A');
    free(d);
}

Test(my_split, basic_split)
{
    char **result = my_split("a:b:c", ":");

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_str_eq(result[2], "c");
    cr_assert_null(result[3]);
    for (int i = 0; result[i]; i++)
        free(result[i]);
    free(result);
}

Test(my_split, multiple_separators)
{
    char **result = my_split("a:b/c", ":/");

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_str_eq(result[2], "c");
    cr_assert_null(result[3]);
    for (int i = 0; result[i]; i++)
        free(result[i]);
    free(result);
}

Test(my_split, no_separator_in_string)
{
    char **result = my_split("hello", ":");

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "hello");
    cr_assert_null(result[1]);
    for (int i = 0; result[i]; i++)
        free(result[i]);
    free(result);
}

Test(my_split, null_str_returns_null)
{
    cr_assert_null(my_split(NULL, ":"));
}

Test(my_split, null_sep_returns_null)
{
    cr_assert_null(my_split("hello", NULL));
}

Test(my_split, consecutive_separators_skipped)
{
    char **result = my_split("a::b", ":");

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_null(result[2]);
    for (int i = 0; result[i]; i++)
        free(result[i]);
    free(result);
}

Test(my_split, empty_string_returns_empty_array)
{
    char **result = my_split("", ":");

    cr_assert_not_null(result);
    cr_assert_null(result[0]);
    free(result);
}

Test(my_split, only_separators)
{
    char **result = my_split(":::", ":");

    cr_assert_not_null(result);
    cr_assert_null(result[0]);
    free(result);
}

Test(my_str_to_word_array, basic_space_split)
{
    char **result = my_str_to_word_array("hello world", ' ');

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "hello");
    cr_assert_str_eq(result[1], "world");
    cr_assert_null(result[2]);
    for (int i = 0; result[i]; i++)
        free(result[i]);
    free(result);
}

Test(my_str_to_word_array, leading_spaces)
{
    char **result = my_str_to_word_array("  hello", ' ');

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "hello");
    cr_assert_null(result[1]);
    for (int i = 0; result[i]; i++)
        free(result[i]);
    free(result);
}

Test(my_str_to_word_array, trailing_spaces)
{
    char **result = my_str_to_word_array("hello  ", ' ');

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "hello");
    cr_assert_null(result[1]);
    for (int i = 0; result[i]; i++)
        free(result[i]);
    free(result);
}

Test(my_str_to_word_array, multiple_words)
{
    char **result = my_str_to_word_array("one two three", ' ');

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "one");
    cr_assert_str_eq(result[1], "two");
    cr_assert_str_eq(result[2], "three");
    cr_assert_null(result[3]);
    for (int i = 0; result[i]; i++)
        free(result[i]);
    free(result);
}

Test(my_str_to_word_array, word_count_correct)
{
    char **result = my_str_to_word_array("a b c d", ' ');
    int count = 0;

    cr_assert_not_null(result);
    while (result[count])
        count++;
    cr_assert_eq(count, 4);
    for (int i = 0; result[i]; i++)
        free(result[i]);
    free(result);
}

Test(my_str_to_word_array, single_word_no_separator)
{
    char **result = my_str_to_word_array("hello", ':');

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "hello");
    cr_assert_null(result[1]);
    free(result[0]);
    free(result);
}

Test(my_str_to_word_array, empty_string_null_first)
{
    char **result = my_str_to_word_array("", ' ');

    cr_assert_not_null(result);
    cr_assert_null(result[0]);
    free(result);
}

Test(my_str_to_word_array, tab_is_separator)
{
    char **result = my_str_to_word_array("a\tb", ' ');

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_null(result[2]);
    free(result[0]);
    free(result[1]);
    free(result);
}

Test(my_str_to_word_array, newline_is_separator)
{
    char **result = my_str_to_word_array("a\nb", ' ');

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_null(result[2]);
    free(result[0]);
    free(result[1]);
    free(result);
}
