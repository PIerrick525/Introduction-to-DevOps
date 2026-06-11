/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2-9
** File description:
** init_free_bonus
*/

#include "mysh_bonus.h"

cmd_bonus_t *init_cmd_bonus(void)
{
    cmd_bonus_t *cmd = malloc(sizeof(cmd_bonus_t));

    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->appendfile = NULL;
    cmd->heredoc_del = NULL;
    cmd->errfile = NULL;
    cmd->errappendfile = NULL;
    cmd->stderr_to_stdout = 0;
    cmd->stdout_to_stderr = 0;
    return cmd;
}

void cmd_bonus_free(cmd_bonus_t *cmd)
{
    if (!cmd)
        return;
    free_tokens(cmd->args);
    free(cmd->infile);
    free(cmd->outfile);
    free(cmd->appendfile);
    free(cmd->heredoc_del);
    free(cmd->errfile);
    free(cmd->errappendfile);
    free(cmd);
}
