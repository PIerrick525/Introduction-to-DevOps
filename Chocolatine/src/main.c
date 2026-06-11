/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell1-16
** File description:
** main
*/

#include "mysh.h"

int main(int ac, char **av, char **envp)
{
    shell_t shell;
    int ret;

    (void)ac;
    (void)av;
    shell.env = env_copy(envp);
    shell.last_exit = 0;
    shell.line = NULL;
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    ret = loop(&shell);
    env_free(shell.env);
    return ret;
}
