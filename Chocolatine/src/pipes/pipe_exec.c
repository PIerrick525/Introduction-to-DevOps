/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** pipe_exec
*/

#include "mysh.h"
#include <signal.h>

static int count_cmds(char **cmds)
{
    int i = 0;

    while (cmds[i])
        i++;
    return i;
}

static void child_io(int **pipes, int i, int n)
{
    if (i > 0)
        dup2(pipes[i - 1][0], STDIN_FILENO);
    if (i < n - 1)
        dup2(pipes[i][1], STDOUT_FILENO);
}

static void close_pipes(int **pipes, int n)
{
    int i = 0;

    while (i < n - 1) {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

static void run_child(char **cmds, int **pipes, int i, shell_t *shell)
{
    cmd_t *cmd;

    signal(SIGPIPE, SIG_DFL);
    child_io(pipes, i, count_cmds(cmds));
    close_pipes(pipes, count_cmds(cmds));
    cmd = parse_cmd(cmds[i]);
    if (!cmd || !cmd->args || !cmd->args[0])
        exit(84);
    if (cmd->infile || cmd->outfile || cmd->appendfile || cmd->heredoc_del)
        exit(exec_cmd(cmd, shell));
    exit(execute(cmd->args, shell));
}

static int **alloc_pipes(int n)
{
    int **pipes = malloc(sizeof(int *) * (n - 1));
    int i = 0;

    if (!pipes)
        return NULL;
    while (i < n - 1) {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i] || pipe(pipes[i]) == -1)
            return NULL;
        i++;
    }
    return pipes;
}

static void free_pipes(int **pipes, int n, pid_t *pids)
{
    int i = 0;

    while (i < n - 1) {
        free(pipes[i]);
        i++;
    }
    free(pipes);
    free(pids);
}

static int wait_all(pid_t *pids, int n)
{
    int status = 0;
    int last = 0;
    int i = 0;

    while (i < n) {
        waitpid(pids[i], &status, 0);
        if (i == n - 1 && WIFEXITED(status))
            last = WEXITSTATUS(status);
        i++;
    }
    return last;
}

int pipe_exec(char **cmds, shell_t *shell)
{
    int n = count_cmds(cmds);
    int **pipes = alloc_pipes(n);
    pid_t *pids = malloc(sizeof(pid_t) * n);
    int i = 0;
    int ret;

    if (!pipes || !pids)
        return 84;
    signal(SIGPIPE, SIG_IGN);
    while (i < n) {
        pids[i] = fork();
        if (pids[i] == 0)
            run_child(cmds, pipes, i, shell);
        i++;
    }
    close_pipes(pipes, n);
    ret = wait_all(pids, n);
    signal(SIGPIPE, SIG_DFL);
    free_pipes(pipes, n, pids);
    return ret;
}
