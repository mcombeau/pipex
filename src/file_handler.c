/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 18:21:42 by mcombeau          #+#    #+#             */
/*   Updated: 2022/04/29 11:19:40 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* get_input_file:
*	Sets the input file descriptor for pipex.
*	If heredoc is specified, a temporary file will be created,
*	otherwise the specified input file will be opened.
*/
void	get_input_file(t_data *d)
{
	if (d->heredoc == 1)
	{
		get_heredoc(d);
		d->fd_in = open(".heredoc.tmp", O_RDONLY);
		if (d->fd_in == -1)
			exit_error(msg("here_doc", ": ", strerror(errno), 1), d);
	}
	else
	{
		d->fd_in = open(d->av[1], O_RDONLY, 644);
		if (d->fd_in == -1)
			msg(strerror(errno), ": ", d->av[1], 1);
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
		msg(strerror(errno), ": ", d->av[d->ac - 1], 1);
}

/* get_heredoc:
*	Reads from standard input and stores read lines in a temporary
*	file until it finds the specified limiter.
*	Then, it sets pipex's input file descriptor as that temporary
*	file.
*/
void	get_heredoc(t_data *d)
{
	int		tmp_fd;
	int		stdin_fd;
	char	*line;

	tmp_fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	stdin_fd = dup(STDIN_FILENO);
	if (tmp_fd == -1)
		exit_error(msg("here_doc", ": ", strerror(errno), 1), d);
	line = "";
	while (1)
	{
		ft_putstr_fd("here_doc > ", 1);
		line = get_next_line(stdin_fd);
		if (line == NULL)
			break ;
		if (ft_strlen(d->av[2]) + 1 == ft_strlen(line)
			&& !ft_strncmp(line, d->av[2], ft_strlen(d->av[2] + 1)))
			close(stdin_fd);
		else
			ft_putstr_fd(line, tmp_fd);
		free(line);
	}
	close(tmp_fd);
}
