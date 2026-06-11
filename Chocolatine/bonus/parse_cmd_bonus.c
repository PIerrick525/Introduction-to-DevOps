/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** parse_cmd_bonus
*/

#include "mysh_bonus.h"

static int apply_base(cmd_bonus_t *cmd, char **toks, int i)
{
    if (my_strcmp(toks[i], ">") == 0 && toks[i + 1]) {
        cmd->outfile = my_strdup(toks[i + 1]);
        return 2;
    }
    if (my_strcmp(toks[i], ">>") == 0 && toks[i + 1]) {
        cmd->appendfile = my_strdup(toks[i + 1]);
        return 2;
    }
    if (my_strcmp(toks[i], "<") == 0 && toks[i + 1]) {
        cmd->infile = my_strdup(toks[i + 1]);
        return 2;
    }
    if (my_strcmp(toks[i], "<<") == 0 && toks[i + 1]) {
        cmd->heredoc_del = my_strdup(toks[i + 1]);
        return 2;
    }
    return 1;
}

static int apply_fd_alias(cmd_bonus_t *cmd, const char *tok)
{
    if (my_strcmp(tok, "2>&1") == 0) {
        cmd->stderr_to_stdout = 1;
        return 1;
    }
    cmd->stdout_to_stderr = 1;
    return 1;
}

static int apply_bonus(cmd_bonus_t *cmd, char **toks, int i)
{
    if (my_strcmp(toks[i], "2>&1") == 0
        || my_strcmp(toks[i], "1>&2") == 0)
        return apply_fd_alias(cmd, toks[i]);
    if (my_strcmp(toks[i], "2>") == 0 && toks[i + 1]) {
        cmd->errfile = my_strdup(toks[i + 1]);
        return 2;
    }
    if (my_strcmp(toks[i], "2>>") == 0 && toks[i + 1]) {
        cmd->errappendfile = my_strdup(toks[i + 1]);
        return 2;
    }
    return 1;
}

static int apply_redirect(cmd_bonus_t *cmd, char **toks, int i)
{
    if (is_bonus_redirect(toks[i]))
        return apply_bonus(cmd, toks, i);
    return apply_base(cmd, toks, i);
}

static int count_args(char **toks)
{
    int i = 0;
    int count = 0;

    while (toks[i]) {
        if (is_redirect(toks[i]))
            i += is_single_token_redirect(toks[i]) ? 1 : 2;
        else {
            count++;
            i++;
        }
    }
    return count;
}

static void fill_args(cmd_bonus_t *cmd, char **toks)
{
    int i = 0;
    int j = 0;
    int skip;
    char *arg;

    while (toks[i]) {
        if (is_redirect(toks[i])) {
            skip = apply_redirect(cmd, toks, i);
            i += skip;
        } else {
            arg = my_strdup(toks[i]);
            cmd->args[j] = arg;
            j++;
            i++;
        }
    }
    cmd->args[j] = NULL;
}

static cmd_bonus_t *build_cmd(char **toks)
{
    cmd_bonus_t *cmd = init_cmd_bonus();
    int argc;

    if (!cmd)
        return NULL;
    argc = count_args(toks);
    cmd->args = malloc(sizeof(char *) * (argc + 1));
    if (!cmd->args) {
        free(cmd);
        return NULL;
    }
    fill_args(cmd, toks);
    return cmd;
}

cmd_bonus_t *parse_cmd_bonus(const char *str)
{
    char **toks = my_str_to_word_array(str, ' ');
    cmd_bonus_t *cmd;

    if (!toks)
        return NULL;
    cmd = build_cmd(toks);
    free_tokens(toks);
    return cmd;
}
