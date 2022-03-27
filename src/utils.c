#include "../includes/pipex.h"

/* exit_error:
*   In case of error, prints an error message and closes any open
*   file descriptors.
*/
void    exit_error(int error_status, t_data *data)
{
    if (data)
        close_fds(data);
    exit(error_status);
}

int error_msg(char *str1, char *str2, char *str3, int erno)
{
    ft_putstr_fd("pipex: ", 2);
    ft_putstr_fd(str1, 2);
    ft_putstr_fd(str2, 2);
    ft_putendl_fd(str3, 2);
    return(erno);
}

/* close_fds:
*   Used to close any open file descriptors in case of error.
*/
void    close_fds (t_data *data)
{
    if (data->fd_in != -1)
        close(data->fd_in);
    if (data->fd_out != -1)
        close(data->fd_out);
    if (data->pipe_fd[0] != -1)
        close(data->pipe_fd[0]);
    if (data->pipe_fd[1] != -1)
        close(data->pipe_fd[1]);
}

/* close_pipe_fds:
*   Closes the pipe read and write file descriptors.
*/
void    close_pipe_fds(t_data *data)
{
    if (close(data->pipe_fd[0]) == -1)
        exit_error(error_msg("Close pipe_fd[0]", ": ", strerror(errno), EXIT_FAILURE), data);
    if (close(data->pipe_fd[1]) == -1)
        exit_error(error_msg("Close pipe_fd", ": ", strerror(errno), EXIT_FAILURE), data);
}

/* free_strs:
*   Frees a string and/or an array of strings if they are not already NULL.
*/
void    free_strs(char *str, char **strs)
{
    int i;
    
    if (str != NULL)
    {
        free(str);
        str = NULL;
    }
    if (strs != NULL)
    {
        i = 0;
        while (strs[i])
        {
            free(strs[i]);
            i++;
        }
        free(strs);
        strs = NULL;
    }
}