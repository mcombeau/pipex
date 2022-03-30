/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 18:21:42 by mcombeau          #+#    #+#             */
/*   Updated: 2022/03/29 18:56:15 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

/* get_input_file:
*	Sets the input file descriptor for pipex.
*	If heredoc is specified, a temporary file will be created,
*	otherwise the specified input file will be opened.
*/
void	get_input_file(t_data *d)
{
	if (d->heredoc == 1)
		get_heredoc(d);
	else
	{
		d->fd_in = open(d->av[1], O_RDONLY, 644);
		if (d->fd_in == -1)
			msg(d->av[1], ": ", strerror(errno), 1);
	}
}

/* get_output_file:
*	Opens/creates an output file. If here_doc is specified, the
*	output file will be appended, otherwise it will be overwritten.
*/
void	get_output_file(t_data *d)
{
	if (d->heredoc == 1)
		d->fd_out = open(d->av[d->ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		d->fd_out = open(d->av[d->ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (d->fd_out == -1)
		msg(d->av[d->ac - 1], ": ", strerror(errno), 1);
}

/* get_heredoc:
*	Reads from standard input and stores read lines in a temporary
*	file until it finds the specified limiter.
*	Then, it sets pipex's input file descriptor as that temporary
*	file.
*/
void	get_heredoc(t_data *data)
{
	int		tmp_fd;
	char	*line;

	tmp_fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp_fd == -1)
		exit_error(msg("here_doc", ": ", strerror(errno), 1), data);
	line = "";
	while (1)
	{
		ft_putstr_fd("here_doc > ", 1);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			exit(1);
		if (ft_strlen(data->av[2]) + 1 == ft_strlen(line)
			&& !ft_strncmp(line, data->av[2], ft_strlen(data->av[2] + 1)))
			break ;
		ft_putstr_fd(line, tmp_fd);
		free(line);
	}
	close(tmp_fd);
	data->fd_in = open(".heredoc.tmp", O_RDONLY);
	if (data->fd_in == -1)
		exit_error(msg("here_doc", ": ", strerror(errno), 1), data);
}
