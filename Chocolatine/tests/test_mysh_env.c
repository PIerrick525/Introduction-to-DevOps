/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** test_mysh_env
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

Test(env_helpers, get_existing_key)
{
    char *raw[] = {"HOME=/root", "PATH=/bin:/usr/bin", NULL};
    env_t *env = env_copy(raw);

    cr_assert_str_eq(env_get(env, "HOME"), "/root");
    cr_assert_str_eq(env_get(env, "PATH"), "/bin:/usr/bin");
    env_free(env);
}

Test(env_helpers, get_missing_key)
{
    char *raw[] = {"HOME=/root", NULL};
    env_t *env = env_copy(raw);

    cr_assert_null(env_get(env, "MISSING_VAR_XYZ"));
    env_free(env);
}

Test(env_helpers, set_new_key)
{
    char *raw[] = {"HOME=/root", NULL};
    env_t *env = env_copy(raw);

    cr_assert_eq(env_set(&env, "MYVAR", "hello"), 0);
    cr_assert_str_eq(env_get(env, "MYVAR"), "hello");
    env_free(env);
}

Test(env_helpers, set_overwrite_existing)
{
    char *raw[] = {"LVL=1", NULL};
    env_t *env = env_copy(raw);

    cr_assert_eq(env_set(&env, "LVL", "4"), 0);
    cr_assert_str_eq(env_get(env, "LVL"), "4");
    env_free(env);
}

Test(env_helpers, unset_existing)
{
    char *raw[] = {"HOME=/root", "TMP=/tmp", NULL};
    env_t *env = env_copy(raw);

    env_unset(&env, "TMP");
    cr_assert_null(env_get(env, "TMP"));
    env_free(env);
}

Test(env_helpers, unset_nonexistent_no_crash)
{
    char *raw[] = {"HOME=/root", NULL};
    env_t *env = env_copy(raw);

    env_unset(&env, "GHOST");
    cr_assert_not_null(env_get(env, "HOME"));
    env_free(env);
}

Test(env_helpers, env_to_array_not_null)
{
    char *raw[] = {"FOO=bar", "BAZ=qux", NULL};
    env_t *env = env_copy(raw);
    char **arr = env_to_array(env);

    cr_assert_not_null(arr);
    cr_assert_not_null(arr[0]);
    env_free_array(arr);
    env_free(env);
}

Test(env_helpers, new_node_key_value)
{
    env_t *node = env_new_node("MYKEY=myval");

    cr_assert_not_null(node);
    cr_assert_str_eq(node->key, "MYKEY");
    cr_assert_str_eq(node->value, "myval");
    free(node->key);
    free(node->value);
    free(node);
}

Test(env_helpers, new_node_value_preserves_content)
{
    env_t *node = env_new_node("MYKEY=hello");

    cr_assert_not_null(node);
    cr_assert_str_eq(node->key, "MYKEY");
    cr_assert_str_eq(node->value, "hello");
    free(node->key);
    free(node->value);
    free(node);
}

Test(env_helpers, new_node_value_with_equals)
{
    env_t *node = env_new_node("LS=Color=12");

    cr_assert_not_null(node);
    cr_assert_str_eq(node->key, "LS",
        "key must be LS, got: %s", node->key);
    cr_assert_str_eq(node->value, "Color=12",
        "value must be Color=12, got: %s", node->value);
    free(node->key);
    free(node->value);
    free(node);
}

Test(env_helpers, new_node_path_with_colons)
{
    env_t *node = env_new_node("PATH=/bin:/usr/bin:/usr/local/bin");

    cr_assert_not_null(node);
    cr_assert_str_eq(node->key, "PATH");
    cr_assert_str_eq(node->value, "/bin:/usr/bin:/usr/local/bin");
    free(node->key);
    free(node->value);
    free(node);
}

Test(env_helpers, new_node_empty_value)
{
    env_t *node = env_new_node("EMPTY=");

    cr_assert_not_null(node);
    cr_assert_str_eq(node->key, "EMPTY");
    cr_assert_str_eq(node->value, "");
    free(node->key);
    free(node->value);
    free(node);
}

Test(env_helpers, new_node_double_equals)
{
    env_t *node = env_new_node("LS=Color=12");

    cr_assert_not_null(node);
    cr_assert_str_eq(node->key, "LS",
        "key must be LS, got: %s", node->key);
    cr_assert_str_eq(node->value, "Color=12",
        "value must preserve second =, got: %s", node->value);
    free(node->key);
    free(node->value);
    free(node);
}

Test(env_helpers, new_node_triple_equals)
{
    env_t *node = env_new_node("A=b=c=d");

    cr_assert_not_null(node);
    cr_assert_str_eq(node->key, "A");
    cr_assert_str_eq(node->value, "b=c=d",
        "value must preserve all after first =, got: %s", node->value);
    free(node->key);
    free(node->value);
    free(node);
}

Test(env_helpers, env_copy_preserves_equals_in_value)
{
    char *raw[] = {"LS=Color=12", "PATH=/bin:/usr/bin", NULL};
    env_t *env = env_copy(raw);

    cr_assert_str_eq(env_get(env, "LS"), "Color=12",
        "env_copy must preserve value with = inside, got: %s",
        env_get(env, "LS"));
    env_free(env);
}

Test(env_helpers, new_node_null_returns_null)
{
    cr_assert_null(env_new_node(NULL));
}

Test(env_helpers, new_node_empty_string_returns_null)
{
    cr_assert_null(env_new_node(""));
}

Test(env_helpers, new_node_no_equals_sign)
{
    env_t *node = env_new_node("NOEQUALS");

    cr_assert_not_null(node);
    cr_assert_str_eq(node->key, "NOEQUALS");
    cr_assert_str_eq(node->value, "");
    free(node->key);
    free(node->value);
    free(node);
}

Test(env_helpers, env_copy_null_returns_null)
{
    cr_assert_null(env_copy(NULL));
}

Test(env_helpers, env_copy_empty_array)
{
    char *raw[] = {NULL};
    env_t *env = env_copy(raw);

    cr_assert_null(env);
}

Test(env_helpers, env_copy_multiple_nodes)
{
    char *raw[] = {"A=1", "B=2", "C=3", NULL};
    env_t *env = env_copy(raw);

    cr_assert_str_eq(env_get(env, "A"), "1");
    cr_assert_str_eq(env_get(env, "B"), "2");
    cr_assert_str_eq(env_get(env, "C"), "3");
    env_free(env);
}

Test(env_helpers, env_get_null_env)
{
    cr_assert_null(env_get(NULL, "KEY"));
}

Test(env_helpers, env_free_null_no_crash)
{
    env_free(NULL);
    cr_assert(1);
}

Test(env_helpers, env_free_array_null_no_crash)
{
    env_free_array(NULL);
    cr_assert(1);
}

Test(env_helpers, env_to_array_content)
{
    char *raw[] = {"FOO=bar", NULL};
    env_t *env = env_copy(raw);
    char **arr = env_to_array(env);

    cr_assert_not_null(arr);
    cr_assert_str_eq(arr[0], "FOO=bar");
    cr_assert_null(arr[1]);
    env_free_array(arr);
    env_free(env);
}

Test(env_helpers, env_to_array_empty_env)
{
    char **arr = env_to_array(NULL);

    cr_assert_not_null(arr);
    cr_assert_null(arr[0]);
    env_free_array(arr);
}

Test(env_helpers, env_to_array_multiple)
{
    char *raw[] = {"A=1", "B=2", "C=3", NULL};
    env_t *env = env_copy(raw);
    char **arr = env_to_array(env);
    int count = 0;

    cr_assert_not_null(arr);
    while (arr[count])
        count++;
    cr_assert_eq(count, 3);
    env_free_array(arr);
    env_free(env);
}

Test(env_helpers, env_set_on_empty_list)
{
    env_t *env = NULL;

    cr_assert_eq(env_set(&env, "KEY", "val"), 0);
    cr_assert_str_eq(env_get(env, "KEY"), "val");
    env_free(env);
}

Test(env_helpers, env_set_multiple_new_keys)
{
    env_t *env = NULL;

    env_set(&env, "A", "1");
    env_set(&env, "B", "2");
    env_set(&env, "C", "3");
    cr_assert_str_eq(env_get(env, "A"), "1");
    cr_assert_str_eq(env_get(env, "B"), "2");
    cr_assert_str_eq(env_get(env, "C"), "3");
    env_free(env);
}

Test(env_helpers, env_unset_first_node)
{
    char *raw[] = {"FIRST=1", "SECOND=2", NULL};
    env_t *env = env_copy(raw);

    env_unset(&env, "FIRST");
    cr_assert_null(env_get(env, "FIRST"));
    cr_assert_str_eq(env_get(env, "SECOND"), "2");
    env_free(env);
}

Test(env_helpers, env_unset_last_node)
{
    char *raw[] = {"FIRST=1", "LAST=2", NULL};
    env_t *env = env_copy(raw);

    env_unset(&env, "LAST");
    cr_assert_null(env_get(env, "LAST"));
    cr_assert_str_eq(env_get(env, "FIRST"), "1");
    env_free(env);
}

Test(env_helpers, env_unset_middle_node)
{
    char *raw[] = {"A=1", "B=2", "C=3", NULL};
    env_t *env = env_copy(raw);

    env_unset(&env, "B");
    cr_assert_null(env_get(env, "B"));
    cr_assert_str_eq(env_get(env, "A"), "1");
    cr_assert_str_eq(env_get(env, "C"), "3");
    env_free(env);
}

Test(env_helpers, env_unset_only_node)
{
    char *raw[] = {"ONLY=val", NULL};
    env_t *env = env_copy(raw);

    env_unset(&env, "ONLY");
    cr_assert_null(env);
}

Test(env_helpers, free_tokens_null_no_crash)
{
    free_tokens(NULL);
    cr_assert(1);
}

Test(env_helpers, free_tokens_basic)
{
    char **tokens = malloc(sizeof(char *) * 3);

    tokens[0] = strdup("hello");
    tokens[1] = strdup("world");
    tokens[2] = NULL;
    free_tokens(tokens);
    cr_assert(1);
}

Test(builtin_setenv, simple_numeric_lvl)
{
    char *raw[] = {"HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "LVL", "4", NULL};

    cr_assert_eq(builtin_setenv(args, sh), 0);
    cr_assert_str_eq(env_get(sh->env, "LVL"), "4");
    free_shell(sh);
}

Test(builtin_setenv, simple_numeric_mybg)
{
    char *raw[] = {NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "MYBG", "4", NULL};

    cr_assert_eq(builtin_setenv(args, sh), 0);
    cr_assert_str_eq(env_get(sh->env, "MYBG"), "4");
    free_shell(sh);
}

Test(builtin_setenv, value_with_equals_sign)
{
    char *raw[] = {NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "LS", "Color=12", NULL};

    cr_assert_eq(builtin_setenv(args, sh), 0);
    cr_assert_str_eq(env_get(sh->env, "LS"), "Color=12");
    free_shell(sh);
}

Test(builtin_setenv, value_with_colon)
{
    char *raw[] = {NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "PATH", "/bin:/usr/bin", NULL};

    cr_assert_eq(builtin_setenv(args, sh), 0);
    cr_assert_str_eq(env_get(sh->env, "PATH"), "/bin:/usr/bin");
    free_shell(sh);
}

Test(builtin_setenv, overwrite_existing)
{
    char *raw[] = {"PATH=/usr/bin", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "PATH", "/bin:/usr/bin", NULL};

    cr_assert_eq(builtin_setenv(args, sh), 0);
    cr_assert_str_eq(env_get(sh->env, "PATH"), "/bin:/usr/bin");
    free_shell(sh);
}

Test(builtin_setenv, invalid_digit_start, .init = cr_redirect_stderr)
{
    char *raw[] = {NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "BAD SPACE", "val", NULL};
    int ret;

    ret = builtin_setenv(args, sh);
    cr_assert_eq(ret, 84);
    free_shell(sh);
}

Test(builtin_setenv, invalid_dash_in_name, .init = cr_redirect_stderr)
{
    char *raw[] = {NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "BAD-NAME", "val", NULL};
    int ret;

    ret = builtin_setenv(args, sh);
    cr_assert_eq(ret, 84);
    free_shell(sh);
}

Test(builtin_setenv, invalid_empty_name, .init = cr_redirect_stderr)
{
    char *raw[] = {NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "", "val", NULL};
    int ret;

    ret = builtin_setenv(args, sh);
    cr_assert_eq(ret, 84);
    free_shell(sh);
}

Test(builtin_setenv, invalid_special_char, .init = cr_redirect_stderr)
{
    char *raw[] = {NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "BAD!NAME", "val", NULL};
    int ret;

    ret = builtin_setenv(args, sh);
    cr_assert_eq(ret, 84);
    free_shell(sh);
}

Test(builtin_setenv, error_mentions_alphanumeric, .init = cr_redirect_stderr)
{
    char *raw[] = {NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "BAD!NAME", "val", NULL};
    char buf[256];
    FILE *err;
    ssize_t n;

    builtin_setenv(args, sh);
    err = cr_get_redirected_stderr();
    n = fread(buf, 1, sizeof(buf) - 1, err);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "alphanumeric") != NULL,
        "stderr must mention alphanumeric, got: %s", buf);
    free_shell(sh);
}

Test(builtin_setenv, setenv_ls_color_equals)
{
    char *raw[] = {NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "LS", "Color=12", NULL};

    cr_assert_eq(builtin_setenv(args, sh), 0);
    cr_assert_str_eq(env_get(sh->env, "LS"), "Color=12",
        "LS must equal Color=12, got: %s", env_get(sh->env, "LS"));
    free_shell(sh);
}

Test(builtin_setenv, setenv_path_with_colon)
{
    char *raw[] = {NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"setenv", "PATH", "/bin:/usr/bin", NULL};

    cr_assert_eq(builtin_setenv(args, sh), 0);
    cr_assert_str_eq(env_get(sh->env, "PATH"), "/bin:/usr/bin");
    free_shell(sh);
}

Test(builtin_unsetenv, unset_existing_var)
{
    char *raw[] = {"MYVAR=hello", "HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"unsetenv", "MYVAR", NULL};
    int ret;

    ret = builtin_unsetenv(args, sh);
    cr_assert_eq(ret, 0);
    cr_assert_null(env_get(sh->env, "MYVAR"));
    free_shell(sh);
}

Test(builtin_unsetenv, unset_nonexistent_silent)
{
    char *raw[] = {"HOME=/root", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"unsetenv", "GHOST", NULL};
    int ret;

    ret = builtin_unsetenv(args, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(builtin_unsetenv, wildcard_not_supported)
{
    char *raw[] = {"FOO=1", "BAR=2", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"unsetenv", "*", NULL};

    builtin_unsetenv(args, sh);
    cr_assert_not_null(env_get(sh->env, "FOO"),
        "unsetenv '*' must not remove FOO");
    cr_assert_not_null(env_get(sh->env, "BAR"),
        "unsetenv '*' must not remove BAR");
    free_shell(sh);
}

Test(builtin_env, returns_zero)
{
    char *raw[] = {"FOO=bar", "BAZ=qux", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"env", NULL};
    int ret;

    ret = builtin_env(args, sh);
    cr_assert_eq(ret, 0);
    free_shell(sh);
}

Test(builtin_env, prints_vars, .init = cr_redirect_stdout)
{
    char *raw[] = {"FOO=bar", NULL};
    shell_t *sh = make_shell(raw);
    char *args[] = {"env", NULL};
    char buf[1024];
    FILE *out;
    ssize_t n;

    builtin_env(args, sh);
    out = cr_get_redirected_stdout();
    n = fread(buf, 1, sizeof(buf) - 1, out);
    buf[(n > 0) ? n : 0] = '\0';
    cr_assert(strstr(buf, "FOO=bar") != NULL,
        "env output must contain FOO=bar, got: %s", buf);
    free_shell(sh);
}
