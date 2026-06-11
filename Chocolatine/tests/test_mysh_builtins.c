/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** test_mysh_builtins
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
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

Test(builtin_cd, no_home_var_fails, .init = cr_redirect_stderr)
{
    char *raw[] = {"PWD=/", "OLDPWD=", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", NULL};
    char buf[256];
    FILE *err;
    ssize_t n;
    int ret;

    ret = builtin_cd(args, sh);
    cr_assert_neq(ret, 0,
        "cd with no HOME must fail, got %d", ret);
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "home") != NULL || strstr(buf, "HOME") != NULL,
        "error must mention home, got: %s", buf);
    free_shell(sh);
}

Test(builtin_cd, no_args_goes_home)
{
    char *raw[] = {"HOME=/tmp", "PWD=/", "OLDPWD=", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", NULL};
    char buf[4096];
    int ret;

    ret = builtin_cd(args, sh);
    cr_assert_eq(ret, 0, "cd with no args and HOME=/tmp must succeed");
    cr_assert_str_eq(getcwd(buf, sizeof(buf)), "/tmp");
    chdir("/");
    free_shell(sh);
}

Test(builtin_cd, tilde_goes_home)
{
    char *raw[] = {"HOME=/tmp", "PWD=/", "OLDPWD=", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", "~", NULL};
    char buf[4096];
    int ret;

    ret = builtin_cd(args, sh);
    cr_assert_eq(ret, 0, "cd ~ must succeed");
    cr_assert_str_eq(getcwd(buf, sizeof(buf)), "/tmp");
    chdir("/");
    free_shell(sh);
}

Test(builtin_cd, oldpwd_not_set_fails, .init = cr_redirect_stderr)
{
    char *raw[] = {"HOME=/tmp", "PWD=/", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", "-", NULL};
    char buf[256];
    FILE *err;
    ssize_t n;
    int ret;

    ret = builtin_cd(args, sh);
    cr_assert_neq(ret, 0,
        "cd - with no OLDPWD must fail");
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "OLDPWD") != NULL,
        "error must mention OLDPWD, got: %s", buf);
    free_shell(sh);
}

Test(builtin_cd, oldpwd_empty_fails, .init = cr_redirect_stderr)
{
    char *raw[] = {"HOME=/tmp", "PWD=/", "OLDPWD=", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", "-", NULL};
    int ret;

    ret = builtin_cd(args, sh);
    cr_assert_neq(ret, 0,
        "cd - with empty OLDPWD must fail");
    free_shell(sh);
}

Test(builtin_cd, too_many_args_fails, .init = cr_redirect_stderr)
{
    char *raw[] = {"HOME=/tmp", "PWD=/", "OLDPWD=", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", "/tmp", "/var", NULL};
    char buf[256];
    FILE *err;
    ssize_t n;
    int ret;

    ret = builtin_cd(args, sh);
    cr_assert_eq(ret, 84,
        "cd with too many args must return 84, got %d", ret);
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "many") != NULL || strstr(buf, "argument") != NULL,
        "error must mention arguments, got: %s", buf);
    free_shell(sh);
}

Test(builtin_cd, invalid_dir_returns_84, .init = cr_redirect_stderr)
{
    char *raw[] = {"HOME=/tmp", "PWD=/", "OLDPWD=", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", "/no_such_dir_xyz_42", NULL};
    int ret;

    ret = builtin_cd(args, sh);
    cr_assert_eq(ret, 84);
    free_shell(sh);
}

Test(builtin_env, with_argument_returns_84, .init = cr_redirect_stderr)
{
    char *raw[] = {"FOO=bar", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"env", "extra_arg", NULL};
    char buf[256];
    FILE *err;
    ssize_t n;
    int ret;

    ret = builtin_env(args, sh);
    cr_assert_eq(ret, 84,
        "env with argument must return 84, got %d", ret);
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "argument") != NULL || strstr(buf, "many") != NULL,
        "error must mention arguments, got: %s", buf);
    free_shell(sh);
}

Test(builtin_unsetenv, no_args_returns_84, .init = cr_redirect_stderr)
{
    char *raw[] = {"FOO=bar", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"unsetenv", NULL};
    char buf[256];
    FILE *err;
    ssize_t n;
    int ret;

    ret = builtin_unsetenv(args, sh);
    cr_assert_eq(ret, 84,
        "unsetenv with no args must return 84, got %d", ret);
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "argument") != NULL || strstr(buf, "few") != NULL,
        "error must mention arguments, got: %s", buf);
    free_shell(sh);
}

Test(builtin_exit, exits_with_given_code, .exit_code = 42)
{
    char *raw[] = {"HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char **args = malloc(sizeof(char *) * 3);

    args[0] = strdup("exit");
    args[1] = strdup("42");
    args[2] = NULL;
    builtin_exit(args, sh);
}

Test(builtin_exit, exits_with_last_exit_when_no_arg, .exit_code = 7)
{
    char *raw[] = {"HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char **args = malloc(sizeof(char *) * 2);

    sh->last_exit = 7;
    args[0] = strdup("exit");
    args[1] = NULL;
    builtin_exit(args, sh);
}

Test(builtin_exit, exits_with_zero, .exit_code = 0)
{
    char *raw[] = {"HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char **args = malloc(sizeof(char *) * 2);

    sh->last_exit = 0;
    args[0] = strdup("exit");
    args[1] = NULL;
    builtin_exit(args, sh);
}
