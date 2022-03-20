#include "../includes/pipex.h"

void    error(const char *errormsg)
{
    perror(errormsg);
    exit(EXIT_FAILURE);
}

void    free_strs(char *str, char **strs)
{
    int i;
    
    if (str != NULL)
    {
        free(str);
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
    }
}