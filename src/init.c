#include "../includes/pipex.h"

/* clean_init:
*   Initializes a data structure with no data in it. Sets strings to NULL
*   and ints to -1, because 0 is a valid fd and should not be used for
*   initialization.
*   Returns the newly initialized data structure.
*/
t_data  clean_init(void)
{
    t_data data;
    data.envp = NULL;
    data.ac = -1;
    data.av = NULL;
    data.heredoc = 0;
    data.fd_in = -1;
    data.fd_out = -1;
    data.pipe_fd = NULL;
    data.nb_cmds = -1;
    data.child_index = -1;
    return (data);
}

/* generate_pipes:
*   Creates a pipe (pair of fds) for each command.
*/
 void   generate_pipes(t_data *data)
 {
     int    i;

    i = 0;
    while (i < data->nb_cmds - 1)
    {
        if (pipe(data->pipe_fd + 2 * i) == -1)
            exit_error(error_msg("Could not create pipe", "", "", 1), data);
        i++;
    }
}

/* init:
*   Initializes a new data structure to hold pipex information. Stores the
*   arguments, the environment pointer, opens the input fd and opens/creates
*   the output fd.
*   Returns the data structure.
*/
t_data init(int ac, char **av, char **envp)
{
    t_data data;

    data = clean_init();
    data.envp = envp;
    data.ac = ac;
    data.av = av;
    if (!ft_strncmp("here_doc", av[1], 9))
        data.heredoc = 1;
    get_input_file(&data);
    get_output_file(&data);
    data.nb_cmds = ac - 3 - data.heredoc;
    data.pipe_fd = malloc(sizeof * data.pipe_fd * 2 * (data.nb_cmds - 1));
    if (!data.pipe_fd)
        exit_error(error_msg("Could not create pipes", "", "", EXIT_FAILURE), &data);
    generate_pipes(&data);
    return(data);
}