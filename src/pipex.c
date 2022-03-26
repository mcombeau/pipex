#include "../includes/pipex.h"
/* TODO
*   - heredoc
*   - FIX: Wait issue with children.
        If cmd1 has infinite input, cmd2 must be able to execute its part.
        ex: </dev/random cat | head -1
        Issue with /dev/tty, too.
        Rethink av loop in start_pipex, children must start simultaneously,
        not one after the other.
*/

/* child:
*   Sets the child process's outputs correctly, to the standard
*   output if it is the last child, to the pipe write end if not.
*   Then it parses the command it needs to execute and executes it.
*/
void    child(char *cmd, t_data *data, int lastchild)
{
    char **cmd_options;
    char *cmd_path;

    if (lastchild == 1)
    {
        if(dup2(data->fd_out, STDOUT_FILENO) == -1)
            exit_error("Child: Dup2 fd_out STDOUT", data);
    }
    else
        if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
            exit_error("Child: Dup2 pipe_fd[1] STDOUT", data);
    if (close(data->pipe_fd[0]) == -1)
        exit_error("Child: Close pipe_fd[0]", data);
    cmd_options = ft_split(cmd, ' ');
    if (!cmd_options)
        exit_error("Child: Command parsing", data);
    cmd_path = get_cmd(cmd_options[0], data);
    if (!cmd_path)
    {
        free_strs(cmd_path, cmd_options);
        exit_error("Child: Command not found", data);
    }
    if (execve(cmd_path, cmd_options, data->envp) == -1)
    {
        free_strs(cmd_path, cmd_options);
        exit_error("Child: Execve", data);
    }
}

/* pipex:
*   Creates a pipe and forks a child process, while it waits for the child
*   to finish its tasks, if closes the write end of the pipe and sets
*   the read end of the pipe to the standard input to capture the child's
*   output in the pipe. Once the child is done, it checks its status 
*   and closes the read end of the pipe.
*/
void    pipex(char *cmd, t_data *data, int lastchild)
{
    pid_t   pid;
 //   int     status;

    if (pipe(data->pipe_fd) == -1)
        exit_error("Pipex: Pipe", data);
    pid = fork();
    if (pid == -1)
        exit_error("Pipex: Fork", data);
    else if (pid == 0)
        child(cmd, data, lastchild);
    else
    {
        if (close(data->pipe_fd[1]) == -1)
            exit_error("Pipex: Close pipe_fd[1]", data);
        if (dup2(data->pipe_fd[0], STDIN_FILENO) == -1)
            exit_error("Pipex: Dup2 pipe_fd[0] STDIN", data);
//        perror("Waiting for child");
//        perror(ft_itoa(pid));
        waitpid(pid, NULL, 0);
//        if (wpid == -1)
//            exit_error("Pipex: Waitpid", data);
//        if (!WIFEXITED(status))
//            exit_error("Pipex: WIFEXITED", data);
//        else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
//            exit_error("Pipex: WEXITSTATUS", data);
        if (close(data->pipe_fd[0]) == -1)
            exit_error("Pipex: Close pipe_fd[0]", data);
    }
}

/* start_pipex:
*   Sets the given input file as input. Then launches pipex for each
*   command argument received, marking the last one, because the last one
*   will need to write its output to a file instead of the pipex pipe.
*/
void    start_pipex(t_data *data)
{
    int i;

    if (dup2(data->fd_in, STDIN_FILENO) == -1)
        exit_error("Start_pipex: Dup2 fd_in STDIN", data);
    i = 2;
    while (i < data->ac - 1)
    {
 //       perror(data->av[i]);
        if (i == data->ac - 2)
            pipex(data->av[i], data, 1);
        else
            pipex(data->av[i], data, 0);
        i++;
    }
}

/* main:
*   Launches data structure initialization and starts pipex.
*/
int main(int ac, char **av, char **envp)
{
    t_data data;

    if (ac < 5)
    {
        ft_putendl_fd("Pipex: Usage: ./pipex file1 cmd1 cmd2 file2.", STDOUT_FILENO);
        return(EXIT_SUCCESS);
    }
    data = init(ac, av, envp);
    start_pipex(&data);
    return(EXIT_SUCCESS);
}