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
    data.fd_in = -1;
    data.fd_out = -1;
    data.pipe_fd[0] = -1;
    data.pipe_fd[1] = -1;

    return (data);
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
    data.fd_in = open(av[1], O_RDONLY, 644);
    if (data.fd_in == -1)
        exit_error("Pipex: Could not open input file", &data);
    data.fd_out = open(av[ac - 1], O_WRONLY | O_CREAT, 0777);
    if (data.fd_out == -1)
        exit_error("Pipex: Could not create or open output file", &data);
    return(data);
}