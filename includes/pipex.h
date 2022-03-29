/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 18:22:36 by mcombeau          #+#    #+#             */
/*   Updated: 2022/03/29 18:30:14 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include "../libft/libft.h"
# include "get_next_line.h"

/*  Data    */
typedef struct s_data
{
	char	**envp;
	char	**av;
	int		ac;
	int		heredoc;
	int		fd_in;
	int		fd_out;
	int		*pipe_fd;
	int		nb_cmds;
	int		child_index;
	int		*pids;
}		t_data;

/*  initialization.c    */
t_data	init(int ac, char **av, char **envp);

/*  env_parsing.c   */
char	*get_cmd(char *cmd, t_data *data);

/*  utils.c */
void	exit_error(int error_status, t_data *data);
int		error_msg(char *str1, char *str2, char *str3, int erno);
void	close_fds(t_data *data);
void	close_pipe_fds(t_data *data);
void	free_strs(char *str, char **strs);

/* file_handler.c */
void	get_input_file(t_data *data);
void	get_output_file(t_data *data);
void	get_heredoc(t_data *data);

#endif
