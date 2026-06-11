/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** parse_cmd
*/

#include "mysh.h"

static int is_redirect(const char *tok)
{
    return (my_strcmp(tok, ">") == 0 || my_strcmp(tok, ">>") == 0
        || my_strcmp(tok, "<") == 0 || my_strcmp(tok, "<<") == 0);
}

static int apply_redirect(cmd_t *cmd, char **tokens, int i)
{
    if (my_strcmp(tokens[i], ">") == 0 && tokens[i + 1]) {
        cmd->outfile = my_strdup(tokens[i + 1]);
        return 2;
    }
    if (my_strcmp(tokens[i], ">>") == 0 && tokens[i + 1]) {
        cmd->appendfile = my_strdup(tokens[i + 1]);
        return 2;
    }
    if (my_strcmp(tokens[i], "<") == 0 && tokens[i + 1]) {
        cmd->infile = my_strdup(tokens[i + 1]);
        return 2;
    }
    if (my_strcmp(tokens[i], "<<") == 0 && tokens[i + 1]) {
        cmd->heredoc_del = my_strdup(tokens[i + 1]);
        return 2;
    }
    return 1;
}

static int count_args(char **tokens)
{
    int i = 0;
    int count = 0;

    while (tokens[i]) {
        if (is_redirect(tokens[i]))
            i += 2;
        else {
            count++;
            i++;
        }
    }
    return count;
}

static void fill_args(cmd_t *cmd, char **tokens)
{
    int i = 0;
    int j = 0;
    int skip;

    while (tokens[i]) {
        if (is_redirect(tokens[i])) {
            skip = apply_redirect(cmd, tokens, i);
            i += skip;
        } else {
            cmd->args[j] = my_strdup(tokens[i]);
            j++;
            i++;
        }
    }
    cmd->args[j] = NULL;
}

static cmd_t *init_cmd(void)
{
    cmd_t *cmd = malloc(sizeof(cmd_t));

    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->outfile = NULL;
    cmd->appendfile = NULL;
    cmd->infile = NULL;
    cmd->heredoc_del = NULL;
    return cmd;
}

static cmd_t *build_cmd(char **tokens)
{
    cmd_t *cmd = init_cmd();
    int argc;

    if (!cmd)
        return NULL;
    argc = count_args(tokens);
    cmd->args = malloc(sizeof(char *) * (argc + 1));
    if (!cmd->args) {
        free(cmd);
        return NULL;
    }
    fill_args(cmd, tokens);
    return cmd;
}

cmd_t *parse_cmd(const char *str)
{
    char **tokens = my_str_to_word_array(str, ' ');
    cmd_t *cmd;

    if (!tokens)
        return NULL;
    cmd = build_cmd(tokens);
    free_tokens(tokens);
    return cmd;
}

void cmd_free(cmd_t *cmd)
{
    if (!cmd)
        return;
    free_tokens(cmd->args);
    free(cmd->outfile);
    free(cmd->appendfile);
    free(cmd->infile);
    free(cmd->heredoc_del);
    free(cmd);
}
