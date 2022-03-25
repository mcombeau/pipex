#include "../includes/pipex.h"

/* Testing functions */

void    test_envp_parsing(char **envp)
{
    char *cmd_path;
    char *options[3] = {"ls", "-la", NULL};
    char *cmd = "ls";

    cmd_path = get_cmd(cmd, envp);
    if (!cmd_path)
        exit_error("Error: Command could not be found.\n", NULL);
    printf("Executing command \"ls\", at : %s\n", cmd_path);
    execve(cmd_path, options, envp);
    free_strs(cmd_path, NULL);
    printf("Coucou !");
}