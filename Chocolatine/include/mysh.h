/*
** EPITECH PROJECT, 2025
** G-CPE-110 : Secured
** File description:
** hashtable.h
*/

#ifndef MYSH_H
    #define MYSH_H
    #include "my.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/stat.h>
    #include <dirent.h>
    #include <signal.h>
    #include <errno.h>

    #define PROMPT "$> "
    #define PATH_MAX 4096

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
}env_t;

typedef struct s_shell
{
    env_t *env;
    int last_exit;
    char *line;
} shell_t;

typedef struct s_pipe_ctx {
    char **cmds;
    int **pipes;
    pid_t *pids;
    int n;
} pipe_ctx_t;

typedef struct s_cmd {
    char **args;
    char *infile;
    char *outfile;
    char *appendfile;
    char *heredoc_del;
} cmd_t;

int loop(shell_t *shell);

int execute(char **args, shell_t *shell);
int exec_external(char **args, shell_t *shell);

env_t *env_copy(char **envp);
char *env_get(env_t *env, const char *key);
void env_free(env_t *env);
void env_free_array(char **arr);
char **env_to_array(env_t *env);
env_t *env_new_node(const char *entry);
int env_set(env_t **env, const char *key, const char *value);
void env_unset(env_t **env, const char *key);

char *resolve_path(const char *cmd, env_t *env);

int builtin_exit(char **args, shell_t *shell);
int builtin_cd(char **args, shell_t *shell);
int builtin_env(char **args, shell_t *shell);
int builtin_setenv(char **args, shell_t *shell);
int builtin_unsetenv(char **args, shell_t *shell);
char **split_on(const char *line, char sep);
char *my_strndup(const char *str, int n);
void free_tokens(char **tokens);
int run_line(char *line, shell_t *shell);
int run_pipeline(char *segment, shell_t *shell);
cmd_t *parse_cmd(const char *str);
void cmd_free(cmd_t *cmd);
int exec_cmd(cmd_t *cmd, shell_t *shell);
int pipe_exec(char **cmds, shell_t *shell);
int setup_heredoc(const char *delimiter);
int is_builtin(const char *cmd);
int run_builtin(char **args, shell_t *shell);

#endif /* MYSH_H */
