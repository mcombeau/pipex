/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 18:22:00 by mcombeau          #+#    #+#             */
/*   Updated: 2022/04/24 14:06:51 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* redirect_io:
*	Redirects the input and output file descriptors by duplicating
*	the given fds to the standard input and standard output respectively.
*/
static void	redirect_io(int input, int output, t_data *d)
{
	if (dup2(input, STDIN_FILENO) == -1)
	{
		exit_error(1, d);
	}
	if (dup2(output, STDOUT_FILENO) == -1)
	{
		exit_error(1, d);
	}
}

/* child:
*	Sets the child process's inputs and outputs depending on if it is
*	the first, middle or final child.
*	Then it parses the command it needs to execute and executes it.
*/
static void	child(t_data *d)
{
	if (d->child == 0)
		redirect_io(d->fd_in, d->pipe[1], d);
	else if (d->child == d->nb_cmds - 1)
		redirect_io(d->pipe[2 * d->child - 2], d->fd_out, d);
	else
		redirect_io(d->pipe[2 * d->child - 2], d->pipe[2 * d->child + 1], d);
	close_fds(d);
	if (d->cmd_options == NULL || d->cmd_path == NULL)
		exit_error(1, d);
	if (execve(d->cmd_path, d->cmd_options, d->envp) == -1)
		exit_error(msg(d->cmd_options[0], ": ", strerror(errno), 1), d);
}

/* parent:
*	Waits for the children processes to finish and fetches the status of the last
*	child.
*	Returns the exit status code of the last child process.
*/
static int	parent(t_data *d)
{
	pid_t	wpid;
	int		status;
	int		exit_code;

	close_fds(d);
	d->child--;
	exit_code = 1;
	while (d->child >= 0)
	{
		wpid = waitpid(d->pids[d->child], &status, 0);
		if (wpid == d->pids[d->nb_cmds - 1])
		{
			if ((d->child == (d->nb_cmds - 1)) && WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
		}
		d->child--;
	}
	free(d->pipe);
	free(d->pids);
	return (exit_code);
}

/* pipex:
*	Creates a pipe and forks all necessary child processes before calling
*	the parent to wait for them to finish their tasks.
*	Returns: the last child's exit code.
*/
static int	pipex(t_data *d)
{
	int		exit_code;

	if (pipe(d->pipe) == -1)
		exit_error(msg("pipe", ": ", strerror(errno), 1), d);
	d->child = 0;
	while (d->child < d->nb_cmds)
	{
		d->cmd_options = ft_split(d->av[d->child + 2 + d->heredoc], ' ');
		if (!d->cmd_options)
			exit_error(msg("unexpected error", "", "", 1), d);
		d->cmd_path = get_cmd(d->cmd_options[0], d);
		d->pids[d->child] = fork();
		if (d->pids[d->child] == -1)
			exit_error(msg("fork", ": ", strerror(errno), 1), d);
		else if (d->pids[d->child] == 0)
			child(d);
		free_strs(d->cmd_path, d->cmd_options);
		d->child++;
	}
	exit_code = parent(d);
	if (d->heredoc == 1)
		unlink(".heredoc.tmp");
	return (exit_code);
}

/* main:
*	Launches d structure initialization and starts pipex.
*	Returns the last child's exit code as pipex's exit code.
*/
int	main(int ac, char **av, char **envp)
{
	t_data	d;
	int		exit_code;

	if (ac < 5)
	{
		if (ac >= 2 && !ft_strncmp("here_doc", av[1], 9))
			return (msg("Usage: ",
					"./pipex here_doc LIMITER cmd1 cmd2 ... cmdn file2.",
					"", 1));
		return (msg("Usage: ",
				"./pipex file1 cmd1 cmd2 ... cmdn file2.", "", 1));
	}
	else if (ac < 6 && !ft_strncmp("here_doc", av[1], 9))
		return (msg("Usage: ",
				"./pipex here_doc LIMITER cmd1 cmd2 ... cmdn file2.", "", 1));
	if (!envp || envp[0][0] == '\0')
		exit_error(msg("Unexpected error.", "", "", 1), &d);
	d = init(ac, av, envp);
	exit_code = pipex(&d);
	return (exit_code);
}
