/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** run_pipeline
*/

#include "mysh.h"

static int has_empty_cmd(char **cmds)
{
    int i = 0;

    while (cmds[i]) {
        if (cmds[i][0] == '\0') {
            my_put_error("Incorrect null command.\n");
            return 1;
        }
        i++;
    }
    return 0;
}

static int exec_single(char *segment, shell_t *shell)
{
    cmd_t *cmd = parse_cmd(segment);
    int ret;

    if (!cmd || !cmd->args || !cmd->args[0]) {
        cmd_free(cmd);
        return 0;
    }
    ret = exec_cmd(cmd, shell);
    cmd_free(cmd);
    return ret;
}

int run_pipeline(char *segment, shell_t *shell)
{
    char **cmds = split_on(segment, '|');
    int ret;

    if (!cmds)
        return 84;
    if (has_empty_cmd(cmds)) {
        free_tokens(cmds);
        return 84;
    }
    if (!cmds[1]) {
        ret = exec_single(cmds[0], shell);
        free_tokens(cmds);
        return ret;
    }
    ret = pipe_exec(cmds, shell);
    free_tokens(cmds);
    return ret;
}
