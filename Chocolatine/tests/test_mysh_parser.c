/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** test_mysh_parser
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "../include/mysh.h"

static shell_t *make_shell(char **raw)
{
    shell_t *sh = malloc(sizeof(shell_t));

    sh->env = env_copy(raw);
    sh->last_exit = 0;
    sh->line = NULL;
    return sh;
}

static void free_shell(shell_t *sh)
{
    env_free(sh->env);
    free(sh);
}

Test(my_strndup, basic)
{
    char *s = my_strndup("hello", 3);

    cr_assert_not_null(s);
    cr_assert_str_eq(s, "hel");
    free(s);
}

Test(my_strndup, full_length)
{
    char *s = my_strndup("hello", 5);

    cr_assert_not_null(s);
    cr_assert_str_eq(s, "hello");
    free(s);
}

Test(my_strndup, zero_length)
{
    char *s = my_strndup("hello", 0);

    cr_assert_not_null(s);
    cr_assert_str_eq(s, "");
    free(s);
}

Test(my_strndup, null_terminator)
{
    char *s = my_strndup("hello", 3);

    cr_assert_not_null(s);
    cr_assert_eq(s[3], '\0');
    free(s);
}

Test(parse_cmd, simple_command)
{
    cmd_t *cmd = parse_cmd("echo hello");

    cr_assert_not_null(cmd);
    cr_assert_not_null(cmd->args);
    cr_assert_str_eq(cmd->args[0], "echo");
    cr_assert_str_eq(cmd->args[1], "hello");
    cr_assert_null(cmd->args[2]);
    cmd_free(cmd);
}

Test(parse_cmd, single_word)
{
    cmd_t *cmd = parse_cmd("ls");

    cr_assert_not_null(cmd);
    cr_assert_str_eq(cmd->args[0], "ls");
    cr_assert_null(cmd->args[1]);
    cmd_free(cmd);
}

Test(parse_cmd, redirect_out)
{
    cmd_t *cmd = parse_cmd("echo hi > /tmp/out");

    cr_assert_not_null(cmd);
    cr_assert_str_eq(cmd->args[0], "echo");
    cr_assert_str_eq(cmd->args[1], "hi");
    cr_assert_null(cmd->args[2]);
    cr_assert_not_null(cmd->outfile);
    cr_assert_str_eq(cmd->outfile, "/tmp/out");
    cmd_free(cmd);
}

Test(parse_cmd, redirect_append)
{
    cmd_t *cmd = parse_cmd("echo hi >> /tmp/out");

    cr_assert_not_null(cmd);
    cr_assert_not_null(cmd->appendfile);
    cr_assert_str_eq(cmd->appendfile, "/tmp/out");
    cmd_free(cmd);
}

Test(parse_cmd, redirect_in)
{
    cmd_t *cmd = parse_cmd("cat < /tmp/in");

    cr_assert_not_null(cmd);
    cr_assert_not_null(cmd->infile);
    cr_assert_str_eq(cmd->infile, "/tmp/in");
    cmd_free(cmd);
}

Test(parse_cmd, redirect_heredoc)
{
    cmd_t *cmd = parse_cmd("cat << EOF");

    cr_assert_not_null(cmd);
    cr_assert_not_null(cmd->heredoc_del);
    cr_assert_str_eq(cmd->heredoc_del, "EOF");
    cmd_free(cmd);
}

Test(parse_cmd, no_redirect_fields_null)
{
    cmd_t *cmd = parse_cmd("ls -la");

    cr_assert_not_null(cmd);
    cr_assert_null(cmd->outfile);
    cr_assert_null(cmd->appendfile);
    cr_assert_null(cmd->infile);
    cr_assert_null(cmd->heredoc_del);
    cmd_free(cmd);
}

Test(parse_cmd, multiple_args)
{
    cmd_t *cmd = parse_cmd("cmd a b c d");

    cr_assert_not_null(cmd);
    cr_assert_str_eq(cmd->args[0], "cmd");
    cr_assert_str_eq(cmd->args[1], "a");
    cr_assert_str_eq(cmd->args[2], "b");
    cr_assert_str_eq(cmd->args[3], "c");
    cr_assert_str_eq(cmd->args[4], "d");
    cr_assert_null(cmd->args[5]);
    cmd_free(cmd);
}

Test(cmd_free, null_no_crash)
{
    cmd_free(NULL);
    cr_assert(1);
}

Test(cmd_free, basic)
{
    cmd_t *cmd = parse_cmd("ls");

    cmd_free(cmd);
    cr_assert(1);
}

Test(split_on, basic_semicolon)
{
    char **res = split_on("a;b;c", ';');

    cr_assert_not_null(res);
    cr_assert_str_eq(res[0], "a");
    cr_assert_str_eq(res[1], "b");
    cr_assert_str_eq(res[2], "c");
    cr_assert_null(res[3]);
    free_tokens(res);
}

Test(split_on, single_segment)
{
    char **res = split_on("hello", ';');

    cr_assert_not_null(res);
    cr_assert_str_eq(res[0], "hello");
    cr_assert_null(res[1]);
    free_tokens(res);
}

Test(split_on, trims_spaces)
{
    char **res = split_on("  a  ;  b  ", ';');

    cr_assert_not_null(res);
    cr_assert_str_eq(res[0], "a");
    cr_assert_str_eq(res[1], "b");
    cr_assert_null(res[2]);
    free_tokens(res);
}

Test(split_on, empty_string)
{
    char **res = split_on("", ';');

    cr_assert_not_null(res);
    free_tokens(res);
}

Test(split_on, quote_preserves_separator)
{
    char **res = split_on("echo 'a;b'", ';');

    cr_assert_not_null(res);
    cr_assert_null(res[1],
        "quoted semicolon must not split");
    free_tokens(res);
}

Test(split_on, double_quote_preserves_separator)
{
    char **res = split_on("echo \"a;b\"", ';');

    cr_assert_not_null(res);
    cr_assert_null(res[1],
        "double-quoted semicolon must not split");
    free_tokens(res);
}

Test(split_on, pipe_separator)
{
    char **res = split_on("a|b|c", '|');

    cr_assert_not_null(res);
    cr_assert_str_eq(res[0], "a");
    cr_assert_str_eq(res[1], "b");
    cr_assert_str_eq(res[2], "c");
    cr_assert_null(res[3]);
    free_tokens(res);
}

Test(run_line, single_command)
{
    char *raw[] = {"PATH=/bin:/usr/bin", "HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char line[] = "true";
    int ret;

    ret = run_line(line, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(run_line, empty_line)
{
    char *raw[] = {"PATH=/bin:/usr/bin", "HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char line[] = "   ";
    int ret;

    ret = run_line(line, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(run_line, semicolon_two_commands)
{
    char *raw[] = {"PATH=/bin:/usr/bin", "HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char line[] = "true;true";
    int ret;

    ret = run_line(line, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(run_line, updates_last_exit)
{
    char *raw[] = {"PATH=/bin:/usr/bin", "HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char line[] = "true";

    run_line(line, sh);
    cr_assert_eq(sh->last_exit, 0);
    free_shell(sh);
}

Test(run_line, skips_empty_segments)
{
    char *raw[] = {"PATH=/bin:/usr/bin", "HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char line[] = "true; ;true";
    int ret;

    ret = run_line(line, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(run_line, pipe_at_start_returns_84, .init = cr_redirect_stderr)
{
    char *raw[] = {"PATH=/bin:/usr/bin", "HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char line[] = "|";
    int ret;

    ret = run_line(line, sh);
    cr_assert_eq(ret, 84);
    free_shell(sh);
}

Test(run_line, tab_only_is_empty)
{
    char *raw[] = {"PATH=/bin:/usr/bin", "HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char line[] = "\t\t";
    int ret;

    ret = run_line(line, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(run_line, failing_command_updates_last_exit)
{
    char *raw[] = {"PATH=/bin:/usr/bin", "HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char line[] = "false";
    int ret;

    ret = run_line(line, sh);
    cr_assert_neq(ret, 0);
    cr_assert_neq(sh->last_exit, 0);
    free_shell(sh);
}
