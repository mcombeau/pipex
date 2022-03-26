#include "../includes/pipex.h"

/* exit_error:
*   In case of error, prints an error message and closes any open
*   file descriptors.
*/
void    exit_error(const char *errormsg, t_data *data)
{
    perror(errormsg);
    if (data)
        close_fds(data);
    exit(EXIT_FAILURE);
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