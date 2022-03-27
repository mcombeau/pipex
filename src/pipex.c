#include "../includes/pipex.h"
/* TODO
*   - heredoc
*   - parent in a loop instead of loop in parent???!
        to be able to wait for all children simultaneously instead of hanging
        waiting for one...
*/

/* redirect_io:
*   Redirects the input and output file descriptors by duplicating
*   the given fds to the standard input and standard output respectively.
*/
void    redirect_io(int input, int output, t_data *data)
{
    if (dup2(input, STDIN_FILENO) == -1)
    {
        exit_error(error_msg("Dup2", ": ", strerror(errno), EXIT_FAILURE), data);
    }
	if (dup2(output, STDOUT_FILENO) == -1)
    {
        exit_error(error_msg("Dup2", ": ", strerror(errno), EXIT_FAILURE), data);
    }
}

/* child:
*   Sets the child process's inputs and outputs depending on if it is
*   the first, middle or final child.
*   Then it parses the command it needs to execute and executes it.
*/
 void   child(t_data *data)
{
    char **cmd_options;
    char *cmd_path;

    if (data->child_index == 0)
        redirect_io(data->fd_in, data->pipe_fd[1], data);
    else if (data->child_index == data->nb_cmds - 1)
        redirect_io(data->pipe_fd[0], data->fd_out, data);
    else
        redirect_io(data->pipe_fd[0], data->pipe_fd[1], data);
    close_pipe_fds(data);
    cmd_options = ft_split(data->av[data->child_index + 2], ' ');
    if (!cmd_options)
        exit_error(error_msg("unexpected error", "", "", EXIT_FAILURE), data);
    perror(cmd_options[0]);
    cmd_path = get_cmd(cmd_options[0], data);
    if (!cmd_path)
    {
        free_strs(cmd_path, cmd_options);
        exit_error(error_msg(data->av[data->child_index + 2], ": ", "Command not found", EXIT_FAILURE), data);
    }
    if (execve(cmd_path, cmd_options, data->envp) == -1)
    {
        error_msg(cmd_options[0], ": ", strerror(errno), EXIT_FAILURE);
        free_strs(cmd_path, cmd_options);
        exit_error(EXIT_FAILURE, data);
    }
}

/* parent:
*   Waits for the children processes to finish and fetches the status of the last
*   child.
*   Returns the exit status code of the last child process.
*/
int    parent(pid_t *pid, t_data *data)
{
    pid_t wpid;
    int status;
    int exit_code;

    close_pipe_fds(data);
    printf("Parent: waiting for children...\n");
    data->child_index--;
    exit_code = 1;
    while(data->child_index >= 0)
    {
        printf("Last child is %d\n", pid[data->ac - 4]);
        printf("Waiting for child %d\n", pid[data->child_index]);
        wpid = wait(&status);
//        waitpid(pid[data->child_index], &status, 0);
        printf("Got child %d\n", wpid);
        if (wpid == pid[data->ac - 4])
        {
 //       if ((data->child_index == (data->ac - 4)) && WIFEXITED(status))
            exit_code = WEXITSTATUS(status);
            printf("Got last child.\n");
        }
        printf("Didin't get last child, continuing...\n");
       data->child_index--;
    }
    return (exit_code);
}

/* pipex:
*   Creates a pipe and forks all necessary child processes before calling
*   the parent to wait for them to finish their tasks.
*   Returns: the last child's exit code.
*/
int    pipex(t_data *data)
{
    pid_t   pid[data->nb_cmds];
    int     exit_code;

    if (pipe(data->pipe_fd) == -1)
        exit_error(error_msg("pipe", ": ", strerror(errno), EXIT_FAILURE), data);
    data->child_index = 0;
    while (data->child_index < data->nb_cmds)
    {
        perror(ft_itoa(data->child_index));
        pid[data->child_index] = fork();
        if (pid[data->child_index] == -1)
            exit_error(error_msg("fork", ": ", strerror(errno), EXIT_FAILURE), data);
        else if (pid[data->child_index] == 0)
            child(data);
        data->child_index++;
    }
    perror("Pipex: All children launched.");
    exit_code = parent(pid, data);
    return(exit_code);
}

/* main:
*   Launches data structure initialization and starts pipex.
*   Returns the last child's exit code as pipex's exit code.
*/
int main(int ac, char **av, char **envp)
{
    t_data data;
    int     exit_code;

    if (ac < 5)
        return (error_msg("Usage: ", "./pipex file1 cmd1 cmd2 file2.", "", EXIT_FAILURE));
    data = init(ac, av, envp);
    exit_code = pipex(&data);
    return(exit_code);
}