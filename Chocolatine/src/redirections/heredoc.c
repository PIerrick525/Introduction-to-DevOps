/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** heredoc
*/

#include "mysh.h"

static int write_line(int fd, char *line)
{
    int len = my_strlen(line);

    write(fd, line, len);
    write(fd, "\n", 1);
    return 0;
}

static void read_heredoc(int fd, const char *del)
{
    char *line = NULL;
    size_t n = 0;

    while (getline(&line, &n, stdin) != -1) {
        line[my_strlen(line) - 1] = '\0';
        if (my_strcmp(line, del) == 0)
            break;
        write_line(fd, line);
    }
    free(line);
}

int setup_heredoc(const char *delimiter)
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
        return -1;
    read_heredoc(pipefd[1], delimiter);
    close(pipefd[1]);
    return pipefd[0];
}
