/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** redirect_bonus
*/

#include "mysh_bonus.h"
#include <fcntl.h>

static int open_infile(cmd_bonus_t *cmd)
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

static int open_heredoc(cmd_bonus_t *cmd)
{
    int fd = setup_heredoc(cmd->heredoc_del);

    if (fd == -1)
        return -1;
    dup2(fd, STDIN_FILENO);
    close(fd);
    return 0;
}

static int open_outfile(cmd_bonus_t *cmd)
{
    int flags;
    int fd;

    if (cmd->appendfile) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
        fd = open(cmd->appendfile, flags, 0644);
    } else {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
        fd = open(cmd->outfile, flags, 0644);
    }
    if (fd == -1) {
        my_put_error("redirect: Cannot open output file.\n");
        return -1;
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return 0;
}

static int open_errfile(cmd_bonus_t *cmd)
{
    int flags;
    int fd;

    if (cmd->errappendfile) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
        fd = open(cmd->errappendfile, flags, 0644);
    } else {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
        fd = open(cmd->errfile, flags, 0644);
    }
    if (fd == -1) {
        my_put_error("redirect: Cannot open error file.\n");
        return -1;
    }
    dup2(fd, STDERR_FILENO);
    close(fd);
    return 0;
}

static void apply_fd_aliases(cmd_bonus_t *cmd)
{
    if (cmd->stderr_to_stdout)
        dup2(STDOUT_FILENO, STDERR_FILENO);
    if (cmd->stdout_to_stderr)
        dup2(STDERR_FILENO, STDOUT_FILENO);
}

static int apply_redirections_bonus(cmd_bonus_t *cmd)
{
    if (cmd->heredoc_del && open_heredoc(cmd) == -1)
        return -1;
    if (cmd->infile && open_infile(cmd) == -1)
        return -1;
    if ((cmd->outfile || cmd->appendfile) && open_outfile(cmd) == -1)
        return -1;
    if ((cmd->errfile || cmd->errappendfile) && open_errfile(cmd) == -1)
        return -1;
    apply_fd_aliases(cmd);
    return 0;
}

static int has_redirect_bonus(cmd_bonus_t *cmd)
{
    return cmd->infile || cmd->outfile || cmd->appendfile
        || cmd->heredoc_del || cmd->errfile || cmd->errappendfile
        || cmd->stderr_to_stdout || cmd->stdout_to_stderr;
}

static int child_exec_bonus(cmd_bonus_t *cmd, shell_t *shell)
{
    if (apply_redirections_bonus(cmd) == -1)
        exit(84);
    exit(execute(cmd->args, shell));
}

int exec_cmd_bonus(cmd_bonus_t *cmd, shell_t *shell)
{
    pid_t pid;
    int status;

    if (!has_redirect_bonus(cmd))
        return execute(cmd->args, shell);
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 84;
    }
    if (pid == 0)
        child_exec_bonus(cmd, shell);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 84;
}
