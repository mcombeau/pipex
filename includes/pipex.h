#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include "../libft/libft.h"


/*  env_parsing.c   */
char    *get_cmd(char *cmd, char **envp);

/*  utils.c */
void    error(const char *errormsg);
void    free_strs(char *str, char **strs);

#endif