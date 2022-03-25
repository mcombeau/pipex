#include "../includes/pipex.h"
#include "../libft/libft.h"

/* get_env_path:
*   Parses the envp environment variable to find the "PATH=" line and allocates memory for a copy of it.
*   Returns a pointer to the copy of the path line. NULL if there is none or memory allocation failed.
*/
static char    *get_env_path(char **envp)
{
    char *path;
    int i;

    i = 0;
    path = NULL;
    while (envp[i] != NULL && envp[i][0] != '\0')
    {
        path = ft_strnstr(envp[i], "PATH=", 5);
        if (path)
        {
            path = ft_substr(path, 5, ft_strlen(path));
            break;
        }
        i++;
    }
    return (path);
}

/* make_usable_paths:
*   Appends a '/' character at the end of each path in the environment paths.
*   Returns the modified array of strings.
*/
static char    **make_usable_paths(char **paths)
{
    int     i;
    char    *tmp;

    i = 0;
    tmp = NULL;
    while (paths[i])
    {
        tmp = paths[i];
        paths[i] = ft_strjoin(paths[i], "/");
        free(tmp);
        i++;
    }
    return (paths);
}

/* get_env_paths:
*   Gets the paths from the environment information.
*   Returns an array of strings for the different paths.
*/
static char    **get_env_paths(char **envp)
{
    char    *env_path_str;
    char    **paths;

    env_path_str = get_env_path(envp);
    if (!env_path_str)
        return (NULL);
    paths = ft_split(env_path_str, ':');
    free(env_path_str);
    if (!paths)
        return (NULL);
    paths = make_usable_paths(paths);
    if (!paths)
        return (NULL);
    return (paths);
}

/* get_cmd_paths:
*   Checks each environment path to see if the command binary files are present.
*   Returns the valid command path. NULL if none was found.
*/
static char    *get_cmd_path(char *cmd, char **paths)
{
    int     i;
    char    *cmd_path;

    cmd_path = NULL;
    i = 0;
    while (paths[i])
    {
        cmd_path = ft_strjoin(paths[i], cmd);
        if (!cmd_path)
            exit_error("Could not join path and command!", NULL);
        if (access(cmd_path, F_OK | X_OK) == 0)
            return (cmd_path);
        free_strs(cmd_path, NULL);
        i++;
    }
    if (cmd_path)
        free_strs(cmd_path, NULL);
    return (NULL);
}

/* get_cmd:
*   Gets the given command path from the environment variables.
*   Returns the command path, or NULL if no valid command path was found.
*/
char    *get_cmd(char *cmd, char **envp)
{
    char    **env_paths;
    char    *cmd_path;

    env_paths = get_env_paths(envp);
    if (!env_paths)
        return (NULL);
    cmd_path = get_cmd_path(cmd, env_paths);
    free_strs(NULL, env_paths);
    return (cmd_path);
}