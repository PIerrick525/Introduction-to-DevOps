/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** executor_external
*/

#include "mysh.h"
#include <sys/stat.h>
#include <errno.h>

static int get_exit_status(int status)
{
    int sig;

    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    if (WIFSIGNALED(status)) {
        sig = WTERMSIG(status);
        if (sig == SIGPIPE)
            return 0;
        my_put_error(strsignal(sig));
        my_put_error("\n");
        return (128 + sig);
    }
    return 0;
}

static int check_path(char **args, char *path)
{
    struct stat st;

    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        my_put_error(args[0]);
        my_put_error(": Permission denied.\n");
        return 84;
    }
    if (access(path, X_OK) != 0) {
        my_put_error(args[0]);
        my_put_error(": ");
        my_put_error(strerror(errno));
        my_put_error(".\n");
        return 84;
    }
    return 0;
}

static void run(char *path, char **args, char **envp)
{
    execve(path, args, envp);
    my_put_error(args[0]);
    my_put_error(": Exec format error.\n");
    free(path);
    exit(84);
}

static int wait_child(pid_t pid, char *path, char **envp)
{
    int status;

    free(path);
    env_free_array(envp);
    waitpid(pid, &status, 0);
    return (get_exit_status(status));
}

static int handle_fork_error(char *path, char **envp)
{
    perror("fork");
    free(path);
    env_free_array(envp);
    return (84);
}

static int do_fork(char *path, char **args, char **envp)
{
    pid_t pid = fork();

    if (pid == -1)
        return (handle_fork_error(path, envp));
    if (pid == 0)
        run(path, args, envp);
    return (wait_child(pid, path, envp));
}

int exec_external(char **args, shell_t *shell)
{
    char *path;
    int check;

    path = resolve_path(args[0], shell->env);
    if (!path) {
        my_put_error(args[0]);
        my_put_error(": Command not found.\n");
        return 84;
    }
    check = check_path(args, path);
    if (check != 0) {
        free(path);
        return check;
    }
    return do_fork(path, args, env_to_array(shell->env));
}
