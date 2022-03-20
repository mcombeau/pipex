#include "../includes/pipex.h"



int main(int ac, char **av, char **envp)
{
    char *cmd_path;
    char *options[3] = {"ls", "-la", NULL};
    char *cmd = "ls";

    (void)ac;
    (void)av;
    cmd_path = get_cmd(cmd, envp);
    if (!cmd_path)
        error("Error: Command could not be found.\n");
    printf("Executing command \"ls\", at : %s\n", cmd_path);
    execve(cmd_path, options, envp);
    free_strs(cmd_path, NULL);
    return(0);
}