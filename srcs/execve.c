/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strieste <strieste@student.42.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 15:56:47 by strieste          #+#    #+#             */
/*   Updated: 2025/12/16 11:34:27 by strieste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	child(int fd_in, int fd_out, char **cmd, char **data);
static int	is_valide_cmd(t_data *data, t_cmd **lst_cmd);

int	exec_cmd(t_data *data)
{
	t_cmd	*tmp_c;
	t_redir *tmp_r;
	pid_t	child_t;
	int		pipe_fd[2];
	int		in;
	int		out;
	int		prev_fd;
	int		status;

	tmp_c = data->cmd_lst;
	ft_bzero(pipe_fd, 3);
	in = 0;
	out = 0;
	prev_fd = 0;
	status = 0;
	(void)prev_fd;
	while (tmp_c)
	{
		if (tmp_c->next)
		{
			if (pipe(pipe_fd) == -1)
				return (printf("%sError init PIPE\n%s", RED, NC), 1);	// Error Pipe init exit--code
			if (out)
				out = close(out);
			out = pipe_fd[0];
			prev_fd = pipe_fd[1];
		}
		tmp_r = tmp_c->redir;
		if (tmp_r)
		{
			while (tmp_r)
			{
				if (tmp_r->type == R_IN)
				{
					if (in)
						in = close(in);
					if (access(tmp_r->file, F_OK))
						return (printf("%sError invalide file: %s%s\n", RED, tmp_r->file, NC), 1);
					if (access(tmp_r->file, R_OK))
						return (printf("%sError access file: %s%s\n", RED, tmp_r->file, NC), 1);
					in = open(tmp_r->file, O_RDONLY);
					if (in < 0)
						return (printf("Error open: %s\n", tmp_r->file), 1);
				}
				else if (tmp_r->type == R_OUT)
				{
					if (out)
						out = close(out);		// check return close
					out = open(tmp_r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (out < 0)
						return (printf("Error create: %s\n", tmp_r->file), 1);
				}
				else if (tmp_r->type == R_APPEND)
				{
					if (out)
						out = close(out);		// check return close
					out = open(tmp_r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
					if (out < 0)
						return (printf("Error create or open:%s\n", tmp_r->file), 1);
				}
				tmp_r = tmp_r->next;
			}
		}
		if (is_valide_cmd(data, &tmp_c))		// fd in and out need closed
			return (printf("%sError command path 222:%s%s\n", RED, tmp_c->args[0], NC), 1);	// exit code 127 in function
		child_t = fork();
		if (child_t < 0)
			return (printf("%sError Fork child%s\n", RED, NC), 1);
		if (child_t == 0)			// child function for execve
		{
			if (tmp_c->next)
			{
				close(pipe_fd[0]);
				child(in, out, tmp_c->args, data->envp);
			}
			if (!tmp_c->next)
			{
				close(pipe_fd[1]);
				child(in, out, tmp_c->args, data->envp);
			}
		}
		if (tmp_c->next)
		{
			pipe_fd[1] = close(pipe_fd[1]);
			in = pipe_fd[0];
			waitpid(child_t, &status, 0);		// status check for return child
		}
		else if (!tmp_c->next)
		{
			if (pipe_fd[0])
				pipe_fd[0] = close(pipe_fd[0]);
			in = close(in);
			out = close(out);
			waitpid(child_t, &status, 0);		// status check for return child
		}
		tmp_c = tmp_c->next;
	}
	return (1);
}

static int	is_valide_cmd(t_data *data, t_cmd **lst_cmd)
{
	char	*path_cmd;
	
	path_cmd = find_path((*lst_cmd)->args[0], data->path);
	if (!path_cmd)
		return (printf("%sError path_cmd:%s 120%s\n", RED,(*lst_cmd)->args[0], NC), 1);	// Error find command exit --- code 127 need free data 
	free((*lst_cmd)->args[0]);
	(*lst_cmd)->args[0] = path_cmd;
	return (0);
}

static void	child(int fd_in, int fd_out, char **cmd, char **data)
{
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_in);
	close(fd_out);
	execve(cmd[0], cmd, data);
	perror("execve");
	exit(1); // Need To FREE
}

// void	pipex(t_data *data)
// {
// 	int		pipe_fd[2];
// 	pid_t	child1;
// 	pid_t	child2;
// 	// int		status;

// 	if (pipe(pipe_fd) == -1)
// 		exit(1); // Error function check
// 	child1 = fork();
// 	if (child1 < 0)
// 		return ;	// Error FORK SEE
// 	if (child1 == 0)
// 	{
// 		close(pipe_fd[0]);
// 		close(data->fd_out);
// 		child(data->fd_in, pipe_fd[1], data->cmd_1, data);
// 	}
// 	close(pipe_fd[1]);
// 	child2 = fork();
// 	if (child2 < 0)
// 		return ;
// 	if (child2 == 0)
// 	{
// 		close(pipe_fd[1]);
// 		close(data->fd_in);
// 		child(pipe_fd[0], data->fd_out, data->cmd_2, data);
// 	}
// 	close(pipe_fd[0]);
// 	waitpid(child1, NULL, 0);
// 	return ;
// }

// static void	child(int fd_in, int fd_out, char **cmd, t_data *data)
// {
// 	dup2(fd_in, STDIN_FILENO);
// 	dup2(fd_out, STDOUT_FILENO);
// 	close(fd_in);
// 	close(fd_out);
// 	execve(cmd[0], cmd, data->envp);
// 	// perror("execve");
// 	ft_clean_struct(data);
// 	close(data->fd_in);
// 	close(data->fd_out);
// 	exit(1); // Need To FREE
// }


// while (tmp->redir)
// 			{
// 				if (tmp->redir->type == R_IN)
// 				{
// 					if (in)
// 						close(in);
// 					if (access(tmp->redir->file, F_OK))
// 						return (printf("%sError invalide file: %s%s\n", RED, tmp->redir->file, NC), 1);
// 					if (access(tmp->redir->file, R_OK))
// 						return (printf("%sError access file: %s%s\n", RED, tmp->redir->file, NC), 1);
// 					in = open(tmp->redir->file, O_RDONLY);
// 					if (in < 0)
// 						return (printf("Error open: %s\n", tmp->redir->file), 1);
// 				}
// 				if (tmp->redir->type == R_OUT)
// 				{
// 					if (out)
// 						close(out);
// 					out = open(tmp->redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 					if (out < 0)
// 						return (printf("Error create: %s\n", tmp->redir->file), 1);
// 				}
// 				if (tmp->redir->type == R_APPEND)
// 				{
// 					if (out)
// 						close(out);
// 					out = open(tmp->redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 					if (out < 0)
// 						return (printf("Error create or open:%s\n", tmp->redir->file), 1);
// 				}
// 				tmp->redir = tmp->redir->next;
// 			}