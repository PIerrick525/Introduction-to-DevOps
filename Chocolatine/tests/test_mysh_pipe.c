/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** test_mysh_pipe
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

Test(run_pipeline, single_cmd_true)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char segment[] = "true";
    int ret;

    ret = run_pipeline(segment, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(run_pipeline, single_cmd_false)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char segment[] = "false";
    int ret;

    ret = run_pipeline(segment, sh);
    cr_assert_neq(ret, 0);
    free_shell(sh);
}

Test(run_pipeline, single_cmd_builtin)
{
    char *raw[] = {"FOO=bar", NULL};
    shell_t *sh = make_shell(raw);
    char segment[] = "env";
    int ret;

    ret = run_pipeline(segment, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(run_pipeline, single_empty_segment, .init = cr_redirect_stderr)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char segment[] = "   ";
    int ret;

    ret = run_pipeline(segment, sh);
    cr_assert_eq(ret, 84);
    free_shell(sh);
}

Test(run_pipeline, empty_cmd_in_pipe_returns_84, .init = cr_redirect_stderr)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char segment[] = "echo hi|";
    char buf[256];
    FILE *err;
    ssize_t n;
    int ret;

    ret = run_pipeline(segment, sh);
    cr_assert_eq(ret, 84);
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "null") != NULL || strstr(buf, "Incorrect") != NULL,
        "error must mention null command, got: %s", buf);
    free_shell(sh);
}

Test(run_pipeline, two_cmd_pipe)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char segment[] = "echo hello|cat";
    int ret;

    ret = run_pipeline(segment, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(run_pipeline, pipe_with_grep, .init = cr_redirect_stdout)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char segment[] = "echo hello|grep hello";
    int ret;

    ret = run_pipeline(segment, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(run_pipeline, three_cmd_pipe)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char segment[] = "echo hello|cat|cat";
    int ret;

    ret = run_pipeline(segment, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(run_pipeline, last_exit_status_propagated)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char segment[] = "true|false";
    int ret;

    ret = run_pipeline(segment, sh);
    cr_assert_neq(ret, 0);
    free_shell(sh);
}

Test(run_pipeline, pipe_with_redirect_out)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char segment[] = "echo hello > /tmp/pipe_redirect_test";
    int ret;

    unlink("/tmp/pipe_redirect_test");
    ret = run_pipeline(segment, sh);
    cr_assert_eq(ret, 0);
    cr_assert_eq(access("/tmp/pipe_redirect_test", F_OK), 0);
    unlink("/tmp/pipe_redirect_test");
    free_shell(sh);
}

Test(pipe_exec, two_cmds)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *cmds[] = {"echo hello", "cat", NULL};
    int ret;

    ret = pipe_exec(cmds, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(pipe_exec, three_cmds)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *cmds[] = {"echo hello", "cat", "cat", NULL};
    int ret;

    ret = pipe_exec(cmds, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(pipe_exec, last_status_false)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *cmds[] = {"echo hi", "false", NULL};
    int ret;

    ret = pipe_exec(cmds, sh);
    cr_assert_neq(ret, 0);
    free_shell(sh);
}

Test(pipe_exec, last_status_true)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *cmds[] = {"false", "true", NULL};
    int ret;

    ret = pipe_exec(cmds, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(pipe_exec, cmd_not_found_in_pipe, .init = cr_redirect_stderr)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *cmds[] = {"no_such_cmd_xyz42", "cat", NULL};
    int ret;

    ret = pipe_exec(cmds, sh);
    cr_assert_eq(ret, 0,
        "last command cat succeeds, so exit status is 0");
    free_shell(sh);
}

Test(pipe_exec, builtin_in_pipe)
{
    char *raw[] = {"PATH=/bin:/usr/bin", "FOO=bar", NULL};
    shell_t *sh = make_shell(raw);
    char *cmds[] = {"env", "cat", NULL};
    int ret;

    ret = pipe_exec(cmds, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(pipe_exec, pipe_with_redirect)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *cmds[] = {"echo hello > /tmp/pipe_exec_redir", "cat", NULL};
    int ret;

    unlink("/tmp/pipe_exec_redir");
    ret = pipe_exec(cmds, sh);
    cr_assert_eq(ret, 0);
    unlink("/tmp/pipe_exec_redir");
    free_shell(sh);
}
