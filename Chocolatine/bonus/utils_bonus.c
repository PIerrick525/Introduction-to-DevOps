/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** utils_bonus
*/

#include "mysh_bonus.h"

int is_base_redirect(const char *tok)
{
    return (my_strcmp(tok, ">") == 0 || my_strcmp(tok, ">>") == 0
        || my_strcmp(tok, "<") == 0 || my_strcmp(tok, "<<") == 0);
}

int is_bonus_redirect(const char *tok)
{
    return (my_strcmp(tok, "2>") == 0 || my_strcmp(tok, "2>>") == 0
        || my_strcmp(tok, "2>&1") == 0 || my_strcmp(tok, "1>&2") == 0);
}

int is_redirect(const char *tok)
{
    return is_base_redirect(tok) || is_bonus_redirect(tok);
}

int is_single_token_redirect(const char *tok)
{
    return my_strcmp(tok, "2>&1") == 0 || my_strcmp(tok, "1>&2") == 0;
}
