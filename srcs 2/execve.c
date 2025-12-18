/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strieste <strieste@student.42.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 15:56:47 by strieste          #+#    #+#             */
/*   Updated: 2025/12/17 16:59:45 by strieste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

// static void	child(int fd_in, int fd_out, char **cmd, t_data *data);
static int	is_valide_cmd(t_data *data, t_cmd **lst_cmd);

int	exec_cmd(t_data *data)
{
	t_cmd	*tmp;
	pid_t	*pid;
	int		pipe_fd[2];
	int		status;
	int		prev_fd;
	int		count;
	int		number_cmd;

	tmp = data->cmd_lst;
	prev_fd = -1;
	count = 0;
	number_cmd = lst_size(data->cmd_lst);
	pid = calloc(number_cmd, sizeof(pid_t));
	if (!pid)
		return (printf("Error malloc pid\n"), 1);
	while (tmp)
	{
		ft_bzero(pipe_fd, 2);
		if (tmp->next)
			if (pipe(pipe_fd) == -1)
				return (printf("%sError init PIPE\n%s", RED, NC), 1);	// Error Pipe init exit--code
		pid[count] = fork();	// If BUILTIN command and no pipe execute in parent process no fork
		if (pid[count] < 0)
			return (printf("Error fork\n"), 1);
		if (pid[count] == 0)
		{
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (tmp->next)
			close(pipe_fd[1]);
		prev_fd = tmp->next ? pipe_fd[0] : -1;
		count++;
		tmp = tmp->next;
	}
	number_cmd= 0;
	while (number_cmd < count)
		waitpid(pid[number_cmd++], &status, 0);
	return (0);
}

int	child_process(t_cmd *cmd, t_data *data, int prev_fd, int pipe_fd[2])
{
	t_redir		*tmp;
	struct stat	st;
	int			in;
	int			out;

	in = STDIN_FILENO;
	out = STDOUT_FILENO;
	if (prev_fd != -1)
		in = prev_fd;
	if (cmd->next)
		out = pipe_fd[1];
	/*					APPLY ALL REDIR IF NEEDED				*/
	if (cmd->redir)
	{
		tmp = cmd->redir;
		while (tmp)
		{
			if (tmp->type == R_IN)
			{
				if (in != STDIN_FILENO)
					in = close(in);
				if (access(tmp->file, F_OK))
					return (printf("%sError invalide file: %s%s\n", RED, tmp->file, NC), 1);
				if (access(tmp->file, R_OK))
					return (printf("%sError access file: %s%s\n", RED, tmp->file, NC), 1);
				in = open(tmp->file, O_RDONLY);
				if (in < 0)
					return (printf("Error open: %s\n", tmp->file), 1);		// Error Exit code
			}
			else if (tmp->type == R_OUT)
			{
				if (out != STDOUT_FILENO)
					out = close(out);		// check return close
				out = open(tmp->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (out < 0)
					return (printf("Error create: %s\n", tmp->file), 1);	// Error Exit code 
			}
			else if (tmp->type == R_APPEND)
			{
				if (out != STDOUT_FILENO)
					out = close(out);		// check return close
				out = open(tmp->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (out < 0)
					return (printf("Error create or open:%s\n", tmp->file), 1);	// Errror Exit code 
			}
			tmp = tmp->next;
		}
		/*				CLOSE ALL FD BEFORE CHECK COMMAND			*/
		if (in != STDIN_FILENO)
		{
			dup2(in, STDIN_FILENO);
			if (in == prev_fd)
				prev_fd = -1;
			close(in);
		}
		if (out != STDOUT_FILENO)
		{
			dup2(out, STDOUT_FILENO);
			if (out == pipe_fd[1])
				pipe_fd[1] = -1;
			close(out);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		if (pipe_fd[0])
			close(pipe_fd[0]);		// Need condition for close
		//				Find command in path and if is valide
		/*				CHECK IF IS VALID COMMAND OR BUILTIN IF PIPE	*/
		if (cmd->args[0][0] != '/' && )
			if (find_path(&cmd->args[0], data->path))
			{
				printf("%scommand not found%s\n", RED, NC);
				exit(127);
			}
		/*			Check if is a directory								*/
		if (stat(cmd->args[0], &st) == 0 && IS_ISDIR(st.st_mode))
		{
			perror("is a directoy");
			exit(126);
		}
		execve(cmd->args[0], cmd->args, data->envp);
		/*			If execve fail return Error with errno				*/
		if (errno == ENOENT)	// File or directory does not exist
		{
			printf("%scommand not found%s\n", RED, NC);
			exit(127);
		}
		else if (errno == EACCES)	// Permission denied
		{
			printf("%spermission denied%s\n", RED, NC);
			exit(126);
		}
		else
		{
			perror("minishell");
			exit(1);
		}
	}
	return (0);
}

// int	exec_cmd(t_data *data)
// {
// 	t_cmd	*tmp;
// 	pid_t	child;
// 	int		pipe_fd[2];
// 	int		in;
// 	int		out;
// 	int		status;
	
// 	tmp = data->cmd_lst;
// 	ft_bzero(pipe_fd, 3);
// 	while (tmp)
// 	{
// 		// if (is_valide_cmd(data, &data->cmd_lst))
// 		// 	return (printf("%sCommand not found exec_cmd%s\n", RED, NC), 1);
// 		if (tmp->redir)
// 		{
// 			while (tmp->redir)
// 			{
// 				if (tmp->redir->type == R_IN)
// 				{
// 					if (in)
// 						in = close(in);
// 					if (access(tmp->redir->file, F_OK))
// 						return (printf("%sError invalide file: %s%s\n", RED, tmp->redir->file, NC), 1);
// 					if (access(tmp->redir->file, R_OK))
// 						return (printf("%sError access file: %s%s\n", RED, tmp->redir->file, NC), 1);
// 					in = open(tmp->redir->file, O_RDONLY);
// 					if (in < 0)
// 						return (printf("Error open: %s\n", tmp->redir->file), 1);
// 				}
// 				else if (tmp->redir->type == R_OUT)
// 				{
// 					if (out)
// 						out = close(out);		// check return close
// 					out = open(tmp->redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 					if (out < 0)
// 						return (printf("Error create: %s\n", tmp->redir->file), 1);
// 				}
// 				else if (tmp->redir->type == R_APPEND)
// 				{
// 					if (out)
// 						out = close(out);		// check return close
// 					out = open(tmp->redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 					if (out < 0)
// 						return (printf("Error create or open:%s\n", tmp->redir->file), 1);
// 				}
// 				tmp->redir = tmp->redir->next;
// 			}
// 		}
// 		if (is_valide_cmd(data, &tmp))		// fd in and out need closed
// 		{
// 			return (printf("%sError command path 222:%s%s\n", RED, tmp->args[1], NC), 1);
// 		}	// exit code 127 in function
// 		if (tmp->next)
// 		{
// 			if (pipe(pipe_fd) == -1)
// 				return (printf("%sError init PIPE\n%s", RED, NC), 1);	// Error Pipe init exit--code
// 		}
// 		child = fork();
// 		if (child < 0)
// 			return (printf("%sError Fork child%s\n", RED, NC), 1);
// 		if (child == 0)			// child function for execve
// 		{
// 			dup2(in, STDIN_FILENO);
// 			dup2(out, STDOUT_FILENO);
// 			close(in);
// 			close(out);
// 			execve(tmp->args[0], tmp->args, data->envp);
// 			perror("execve");
// 			exit(1);
// 		}
// 		if (tmp->next)
// 		{
// 			pipe_fd[1] = close(pipe_fd[1]);
// 			in = pipe_fd[0];
// 			waitpid(child, &status, 0);		// status check for return child
// 		}
// 		else if (!tmp->next)
// 		{
// 			if (pipe_fd[0])
// 				pipe_fd[0] = close(pipe_fd[0]);
// 			in = close(in);
// 			out = close(out);
// 			waitpid(child, &status, 0);		// status check for return child
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (1);
// }

static int	is_valide_cmd(t_data *data, t_cmd **lst_cmd)
{
	char	*path_cmd;
	
	path_cmd = find_path((*lst_cmd)->args[0], data->path);
	if (!path_cmd)
		return (printf("%sError path_cmd 120%s\n", RED, NC), 1);	// Error find command exit --- code 127 need free data 
	free((*lst_cmd)->args[0]);
	(*lst_cmd)->args[0] = path_cmd;
	return (0);
}
