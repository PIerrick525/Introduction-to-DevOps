/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-minishell2
** File description:
** mysh_bonus.h
*/

#ifndef MYSH_BONUS_H
    #define MYSH_BONUS_H

    #include "mysh.h"

typedef struct s_cmd_bonus {
    char **args;
    char *infile;
    char *outfile;
    char *appendfile;
    char *heredoc_del;
    char *errfile;
    char *errappendfile;
    int stderr_to_stdout;
    int stdout_to_stderr;
} cmd_bonus_t;

int is_base_redirect(const char *tok);
int is_bonus_redirect(const char *tok);
int is_redirect(const char *tok);
int is_single_token_redirect(const char *tok);
cmd_bonus_t *init_cmd_bonus(void);
void cmd_bonus_free(cmd_bonus_t *cmd);

cmd_bonus_t *parse_cmd_bonus(const char *str);
int exec_cmd_bonus(cmd_bonus_t *cmd, shell_t *shell);

int run_pipeline_bonus(char *segment, shell_t *shell);
int run_line_bonus(char *line, shell_t *shell);

#endif /* MYSH_BONUS_H */
