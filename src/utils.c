/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 18:22:10 by mcombeau          #+#    #+#             */
/*   Updated: 2022/04/24 14:08:04 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* close_pipe_fds:
*	Closes all pipes' read and write file descriptors.
*/
static void	close_pipe_fds(t_data *data)
{
	int	i;

	i = 0;
	while (i < (data->nb_cmds - 1) * 2)
	{
		close(data->pipe[i]);
		i++;
	}
}

/* exit_error:
*	In case of error, prints an error message and closes any open
*	file descriptors.
*/
void	exit_error(int error_status, t_data *data)
{
	if (data)
	{
		close_fds(data);
		if (data->pipe != NULL)
			free(data->pipe);
		if (data->pids != NULL)
			free(data->pids);
		if (data->cmd_options != NULL || data->cmd_path != NULL)
			free_strs(data->cmd_path, data->cmd_options);
	}
	if (data->heredoc == 1)
		unlink(".heredoc.tmp");
	exit(error_status);
}

/* error_msg:
*	Writes an error message to the standard error fd.
*	Returns the error code.
*/
int	msg(char *str1, char *str2, char *str3, int erno)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	ft_putendl_fd(str3, 2);
	return (erno);
}

/* close_fds:
*   Used to close any open file descriptors in case of error.
*/
void	close_fds(t_data *data)
{
	if (data->fd_in != -1)
		close(data->fd_in);
	if (data->fd_out != -1)
		close(data->fd_out);
	close_pipe_fds(data);
}

/* free_strs:
*	Frees a string and/or an array of strings if they are not already NULL.
*/
void	free_strs(char *str, char **strs)
{
	int	i;

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
