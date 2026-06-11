/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** test_mysh_redirections
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

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

static cmd_t *make_cmd(char *arg)
{
    cmd_t *cmd = malloc(sizeof(cmd_t));

    cmd->args = malloc(sizeof(char *) * 2);
    cmd->args[0] = strdup(arg);
    cmd->args[1] = NULL;
    cmd->outfile = NULL;
    cmd->appendfile = NULL;
    cmd->infile = NULL;
    cmd->heredoc_del = NULL;
    return cmd;
}

Test(exec_cmd, no_redirect_runs_builtin)
{
    char *raw[] = {"FOO=bar", NULL};
    shell_t *sh = make_shell(raw);
    cmd_t *cmd = make_cmd("env");
    int ret;

    ret = exec_cmd(cmd, sh);
    cr_assert_eq(ret, 0);
    cmd_free(cmd);
    free_shell(sh);
}

Test(exec_cmd, no_redirect_external)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    cmd_t *cmd = make_cmd("true");
    int ret;

    ret = exec_cmd(cmd, sh);
    cr_assert_eq(ret, 0);
    cmd_free(cmd);
    free_shell(sh);
}

Test(exec_cmd, redirect_out_creates_file)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    cmd_t *cmd = make_cmd("echo");
    int ret;

    unlink("/tmp/test_redirect_out");
    cmd->outfile = strdup("/tmp/test_redirect_out");
    ret = exec_cmd(cmd, sh);
    cr_assert_eq(ret, 0);
    cr_assert_eq(access("/tmp/test_redirect_out", F_OK), 0);
    unlink("/tmp/test_redirect_out");
    cmd_free(cmd);
    free_shell(sh);
}

Test(exec_cmd, redirect_append_creates_file)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    cmd_t *cmd = make_cmd("echo");
    int ret;

    unlink("/tmp/test_redirect_append");
    cmd->appendfile = strdup("/tmp/test_redirect_append");
    ret = exec_cmd(cmd, sh);
    cr_assert_eq(ret, 0);
    cr_assert_eq(access("/tmp/test_redirect_append", F_OK), 0);
    unlink("/tmp/test_redirect_append");
    cmd_free(cmd);
    free_shell(sh);
}

Test(exec_cmd, redirect_in_existing_file)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    cmd_t *cmd = make_cmd("cat");
    FILE *f;
    int ret;

    f = fopen("/tmp/test_redirect_in", "w");
    if (f) {
        fputs("hello\n", f);
        fclose(f);
    }
    cmd->infile = strdup("/tmp/test_redirect_in");
    ret = exec_cmd(cmd, sh);
    cr_assert_eq(ret, 0);
    unlink("/tmp/test_redirect_in");
    cmd_free(cmd);
    free_shell(sh);
}

Test(exec_cmd, redirect_in_missing_file, .init = cr_redirect_stderr)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    cmd_t *cmd = make_cmd("cat");
    char buf[256];
    FILE *err;
    ssize_t n;
    int ret;

    unlink("/tmp/no_such_redirect_file_xyz");
    cmd->infile = strdup("/tmp/no_such_redirect_file_xyz");
    ret = exec_cmd(cmd, sh);
    cr_assert_eq(ret, 84);
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "No such") != NULL ||
        strstr(buf, "directory") != NULL,
        "error must mention missing file, got: %s", buf);
    cmd_free(cmd);
    free_shell(sh);
}

Test(exec_cmd, redirect_out_bad_path, .init = cr_redirect_stderr)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    cmd_t *cmd = make_cmd("echo");
    int ret;

    cmd->outfile = strdup("/no_such_dir_xyz/file");
    ret = exec_cmd(cmd, sh);
    cr_assert_eq(ret, 84);
    cmd_free(cmd);
    free_shell(sh);
}

Test(exec_cmd, redirect_append_bad_path, .init = cr_redirect_stderr)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    cmd_t *cmd = make_cmd("echo");
    int ret;

    cmd->appendfile = strdup("/no_such_dir_xyz/file");
    ret = exec_cmd(cmd, sh);
    cr_assert_eq(ret, 84);
    cmd_free(cmd);
    free_shell(sh);
}

Test(exec_cmd, no_redirect_failing_cmd)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    cmd_t *cmd = make_cmd("false");
    int ret;

    ret = exec_cmd(cmd, sh);
    cr_assert_neq(ret, 0);
    cmd_free(cmd);
    free_shell(sh);
}

Test(setup_heredoc, basic_input)
{
    int pipefd[2];
    int fd;
    char buf[64];
    ssize_t n;

    pipe(pipefd);
    dprintf(pipefd[1], "line1\nEOF\n");
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    fd = setup_heredoc("EOF");
    cr_assert_neq(fd, -1);
    n = read(fd, buf, sizeof(buf) - 1);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "line1") != NULL);
    close(fd);
    freopen("/dev/tty", "r", stdin);
}

Test(setup_heredoc, empty_heredoc)
{
    int pipefd[2];
    int fd;
    char buf[64];
    ssize_t n;

    pipe(pipefd);
    dprintf(pipefd[1], "EOF\n");
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    fd = setup_heredoc("EOF");
    cr_assert_neq(fd, -1);
    n = read(fd, buf, sizeof(buf) - 1);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert_str_eq(buf, "");
    close(fd);
    freopen("/dev/tty", "r", stdin);
}
