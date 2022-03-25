#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include "../libft/libft.h"

/*  Data    */
typedef struct s_data
{
    char    **envp;
    char    **av;
    int     ac;
    int     fd_in;
    int     fd_out;
    int     pipe_fd[2];
}   t_data;

/*  initialization.c    */
t_data init(int ac, char **av, char **envp);

/*  env_parsing.c   */
char    *get_cmd(char *cmd, char **envp);

/*  utils.c */
void    exit_error(const char *errormsg, t_data *data);
void    close_fds(t_data *data);
void    free_strs(char *str, char **strs);

/*  debug.c */
void    test_envp_parsing(char **envp);

#endif