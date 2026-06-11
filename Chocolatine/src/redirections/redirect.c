/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** redirect
*/

#include "mysh.h"
#include <fcntl.h>

static int open_infile(cmd_t *cmd)
{
    int fd = open(cmd->infile, O_RDONLY);

    if (fd == -1) {
        my_put_error(cmd->infile);
        my_put_error(": No such file or directory.\n");
        return -1;
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return 0;
}

static int open_outfile(cmd_t *cmd)
{
    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    int fd;

    if (cmd->appendfile) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
        fd = open(cmd->appendfile, flags, 0644);
    } else {
        fd = open(cmd->outfile, flags, 0644);
    }
    if (fd == -1) {
        my_put_error("redirect: Cannot open file.\n");
        return -1;
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return 0;
}

static int open_heredoc(cmd_t *cmd)
{
    int fd = setup_heredoc(cmd->heredoc_del);

    if (fd == -1)
        return -1;
    dup2(fd, STDIN_FILENO);
    close(fd);
    return 0;
}

static int apply_redirections(cmd_t *cmd)
{
    if (cmd->heredoc_del && open_heredoc(cmd) == -1)
        return -1;
    if (cmd->infile && open_infile(cmd) == -1)
        return -1;
    if ((cmd->outfile || cmd->appendfile) && open_outfile(cmd) == -1)
        return -1;
    return 0;
}

static int has_redirect(cmd_t *cmd)
{
    return cmd->infile || cmd->outfile
        || cmd->appendfile || cmd->heredoc_del;
}

static int child_exec(cmd_t *cmd, shell_t *shell)
{
    if (apply_redirections(cmd) == -1)
        exit(84);
    exit(execute(cmd->args, shell));
}

int exec_cmd(cmd_t *cmd, shell_t *shell)
{
    pid_t pid;
    int status;

    if (!has_redirect(cmd))
        return execute(cmd->args, shell);
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 84;
    }
    if (pid == 0)
        child_exec(cmd, shell);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 84;
}
