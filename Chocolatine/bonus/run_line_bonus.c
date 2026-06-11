/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** run_line_bonus
*/

#include "mysh_bonus.h"

int run_pipeline_bonus(char *segment, shell_t *shell)
{
    char **cmds;
    cmd_bonus_t *cmd;
    int ret;

    cmds = split_on(segment, '|');
    if (!cmds)
        return 84;
    if (cmds[1]) {
        ret = pipe_exec(cmds, shell);
        free_tokens(cmds);
        return ret;
    }
    cmd = parse_cmd_bonus(cmds[0]);
    free_tokens(cmds);
    if (!cmd)
        return 84;
    ret = exec_cmd_bonus(cmd, shell);
    cmd_bonus_free(cmd);
    return ret;
}

int run_line_bonus(char *line, shell_t *shell)
{
    char **segments;
    int ret;
    int i;

    segments = split_on(line, ';');
    if (!segments)
        return 84;
    ret = 0;
    for (i = 0; segments[i]; i++)
        ret = run_pipeline_bonus(segments[i], shell);
    free_tokens(segments);
    return ret;
}
