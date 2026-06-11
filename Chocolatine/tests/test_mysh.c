/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** test_mysh
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

Test(error_handling, exec_directory_returns_84)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"/tmp", NULL};
    int ret;

    ret = execute(args, sh);
    cr_assert_eq(ret, 84,
        "executing a directory must return 84, got %d", ret);
    free_shell(sh);
}

Test(error_handling, exec_non_elf_returns_84)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"/tmp/not_elf_mysh_test", NULL};
    FILE *f;
    int ret;

    f = fopen("/tmp/not_elf_mysh_test", "w");
    if (f != NULL) {
        fputs("not a binary\n", f);
        fclose(f);
    }
    chmod("/tmp/not_elf_mysh_test", 0755);
    ret = execute(args, sh);
    unlink("/tmp/not_elf_mysh_test");
    cr_assert_eq(ret, 84,
        "incompatible binary must return 84, got %d", ret);
    free_shell(sh);
}

Test(error_handling, command_not_found_84)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"this_cmd_does_not_exist_42sh", NULL};
    int ret;

    ret = execute(args, sh);
    cr_assert_eq(ret, 84,
        "unknown command must return 84, got %d", ret);
    free_shell(sh);
}

Test(error_handling, no_permission_returns_84)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"/tmp/no_exec_perm_mysh", NULL};
    FILE *f;
    int ret;

    f = fopen("/tmp/no_exec_perm_mysh", "w");
    if (f != NULL) {
        fputs("#!/bin/sh\necho hi\n", f);
        fclose(f);
    }
    chmod("/tmp/no_exec_perm_mysh", 0644);
    ret = execute(args, sh);
    unlink("/tmp/no_exec_perm_mysh");
    cr_assert_eq(ret, 84,
        "no-permission file must return 84, got %d", ret);
    free_shell(sh);
}

Test(error_handling, directory_error_message, .init = cr_redirect_stderr)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"/tmp", NULL};
    char buf[256];
    FILE *err;
    ssize_t n;

    execute(args, sh);
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "denied") != NULL,
        "error must mention denied, got: %s", buf);
    free_shell(sh);
}

Test(error_handling, non_elf_error_message, .init = cr_redirect_stderr)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"/tmp/non_elf_msg_test", NULL};
    FILE *f;
    char buf[256];
    FILE *err;
    ssize_t n;

    f = fopen("/tmp/non_elf_msg_test", "w");
    if (f) {
        fputs("not a binary\n", f);
        fclose(f);
    }
    chmod("/tmp/non_elf_msg_test", 0755);
    execute(args, sh);
    unlink("/tmp/non_elf_msg_test");
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "Exec format error") != NULL
        || strstr(buf, "format") != NULL,
        "error must mention format, got: %s", buf);
    free_shell(sh);
}

Test(error_handling, directory_returns_84_no_fork)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"/tmp", NULL};
    int ret;

    ret = exec_external(args, sh);
    cr_assert_eq(ret, 84,
        "exec_external on directory must return 84, got %d", ret);
    free_shell(sh);
}

Test(error_handling, non_elf_returns_84_no_fork)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"/tmp/regression_non_elf", NULL};
    FILE *f;
    int ret;

    f = fopen("/tmp/regression_non_elf", "w");
    if (f) {
        fputs("not a binary\n", f);
        fclose(f);
    }
    chmod("/tmp/regression_non_elf", 0755);
    ret = exec_external(args, sh);
    unlink("/tmp/regression_non_elf");
    cr_assert_eq(ret, 84,
        "exec_external on non-ELF must return 84, got %d", ret);
    free_shell(sh);
}

Test(resolve_path_suite, finds_echo_in_path)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    env_t *env = env_copy(raw);
    char *result;

    result = resolve_path("echo", env);
    cr_assert_not_null(result,
        "resolve_path must find echo in PATH");
    free(result);
    env_free(env);
}

Test(resolve_path_suite, absolute_path_unchanged)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    env_t *env = env_copy(raw);
    char *result;

    result = resolve_path("/bin/echo", env);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "/bin/echo");
    free(result);
    env_free(env);
}

Test(resolve_path_suite, unknown_command_null)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    env_t *env = env_copy(raw);
    char *result;

    result = resolve_path("this_cmd_does_not_exist_42sh", env);
    cr_assert_null(result);
    env_free(env);
}

Test(resolve_path_suite, empty_path_null)
{
    char *raw[] = {"PATH=", NULL};
    env_t *env = env_copy(raw);
    char *result;

    result = resolve_path("ls", env);
    cr_assert_null(result);
    env_free(env);
}

Test(resolve_path_suite, absolute_path_nonexec_returned)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    env_t *env = env_copy(raw);
    char *result;

    result = resolve_path("/tmp", env);
    cr_assert_not_null(result,
        "resolve_path must return /tmp even if not a regular exec");
    cr_assert_str_eq(result, "/tmp");
    free(result);
    env_free(env);
}

Test(resolve_path_suite, absolute_nonexistent_returned)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    env_t *env = env_copy(raw);
    char *result;

    result = resolve_path("/no_such_file_xyz", env);
    cr_assert_not_null(result,
        "resolve_path must return path for absolute cmd, let execve fail");
    cr_assert_str_eq(result, "/no_such_file_xyz");
    free(result);
    env_free(env);
}

Test(builtin_cd, cd_to_existing_dir)
{
    char *raw[] = {"HOME=/root", "PWD=/", "OLDPWD=", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", "/tmp", NULL};
    char buf[4096];
    int ret;

    ret = builtin_cd(args, sh);
    cr_assert_eq(ret, 0, "cd /tmp must return 0, got %d", ret);
    cr_assert_str_eq(getcwd(buf, sizeof(buf)), "/tmp");
    chdir("/");
    free_shell(sh);
}

Test(builtin_cd, cd_nonexistent_fails, .init = cr_redirect_stderr)
{
    char *raw[] = {"HOME=/root", "PWD=/", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", "/no_such_dir_42sh", NULL};
    int ret;

    ret = builtin_cd(args, sh);
    cr_assert_neq(ret, 0);
    free_shell(sh);
}

Test(builtin_cd, cd_updates_pwd)
{
    char *raw[] = {"HOME=/root", "PWD=/", "OLDPWD=", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", "/tmp", NULL};

    builtin_cd(args, sh);
    cr_assert_str_eq(env_get(sh->env, "PWD"), "/tmp");
    chdir("/");
    free_shell(sh);
}

Test(builtin_cd, cd_sets_oldpwd)
{
    char *raw[] = {"HOME=/root", "PWD=/", "OLDPWD=", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", "/tmp", NULL};
    char before[4096];

    getcwd(before, sizeof(before));
    builtin_cd(args, sh);
    cr_assert_str_eq(env_get(sh->env, "OLDPWD"), before,
        "OLDPWD must be set to previous cwd");
    chdir(before);
    free_shell(sh);
}

Test(builtin_cd, cd_dash_goes_back)
{
    char *raw[] = {"HOME=/root", "PWD=/tmp", "OLDPWD=/", NULL};
    shell_t *sh = make_shell(raw);
    char *args1[] = {"cd", "/tmp", NULL};
    char *args2[] = {"cd", "-", NULL};
    int ret;

    builtin_cd(args1, sh);
    ret = builtin_cd(args2, sh);
    cr_assert_eq(ret, 0, "cd - must succeed, got %d", ret);
    chdir("/");
    free_shell(sh);
}

Test(executor, is_builtin_cd)
{
    cr_assert_eq(is_builtin("cd"), 1);
}

Test(executor, is_builtin_setenv)
{
    cr_assert_eq(is_builtin("setenv"), 1);
}

Test(executor, is_builtin_unsetenv)
{
    cr_assert_eq(is_builtin("unsetenv"), 1);
}

Test(executor, is_builtin_env)
{
    cr_assert_eq(is_builtin("env"), 1);
}

Test(executor, is_builtin_exit)
{
    cr_assert_eq(is_builtin("exit"), 1);
}

Test(executor, is_builtin_unknown)
{
    cr_assert_eq(is_builtin("ls"), 0);
}

Test(executor, execute_null_args_returns_zero)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    int ret;

    ret = execute(NULL, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(executor, execute_null_first_arg_returns_zero)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {NULL};
    int ret;

    ret = execute(args, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(executor, run_builtin_cd)
{
    char *raw[] = {"HOME=/tmp", "PWD=/", "OLDPWD=", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"cd", "/tmp", NULL};
    int ret;

    ret = run_builtin(args, sh);
    cr_assert_eq(ret, 0);
    chdir("/");
    free_shell(sh);
}

Test(executor, run_builtin_setenv)
{
    char *raw[] = {NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "FOO", "bar", NULL};
    int ret;

    ret = run_builtin(args, sh);
    cr_assert_eq(ret, 0);
    cr_assert_str_eq(env_get(sh->env, "FOO"), "bar");
    free_shell(sh);
}

Test(executor, run_builtin_unsetenv)
{
    char *raw[] = {"FOO=bar", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"unsetenv", "FOO", NULL};
    int ret;

    ret = run_builtin(args, sh);
    cr_assert_eq(ret, 0);
    cr_assert_null(env_get(sh->env, "FOO"));
    free_shell(sh);
}

Test(executor, run_builtin_env)
{
    char *raw[] = {"FOO=bar", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"env", NULL};
    int ret;

    ret = run_builtin(args, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(executor, execute_builtin_dispatched)
{
    char *raw[] = {"FOO=bar", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"env", NULL};
    int ret;

    ret = execute(args, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(executor, execute_external_echo, .init = cr_redirect_stdout)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"echo", "hello", NULL};
    int ret;

    ret = execute(args, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(executor_external, command_not_found, .init = cr_redirect_stderr)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"no_such_cmd_xyz42", NULL};
    char buf[256];
    FILE *err;
    ssize_t n;
    int ret;

    ret = exec_external(args, sh);
    cr_assert_eq(ret, 84);
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "not found") != NULL ||
        strstr(buf, "Command") != NULL,
        "error must mention not found, got: %s", buf);
    free_shell(sh);
}

Test(executor_external, signal_exit_status, .init = cr_redirect_stderr)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"sh", "-c", "kill -TERM $$", NULL};
    int ret;

    ret = execute(args, sh);
    cr_assert_neq(ret, 0);
    free_shell(sh);
}

Test(path, no_path_env_returns_null)
{
    char *raw[] = {NULL};
    env_t *env = env_copy(raw);
    char *result;

    result = resolve_path("ls", env);
    cr_assert_null(result);
    env_free(env);
}

Test(path, cmd_with_slash_returns_strdup)
{
    char *raw[] = {"PATH=/bin:/usr/bin", NULL};
    env_t *env = env_copy(raw);
    char *result;

    result = resolve_path("./mysh", env);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "./mysh");
    free(result);
    env_free(env);
}
