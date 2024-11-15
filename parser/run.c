/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etien <etien@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:51:24 by etien             #+#    #+#             */
/*   Updated: 2024/11/15 11:36:26 by etien            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// TO-DO: CREATE EXEC FUNCTION!!!

// This function will walk the parsing tree and execute the commands.
// If the current node is an EXEC node, it will execute, otherwise
// the function will recursively call itself to continue descending
// down the parsing tree.
void	run_cmd(t_cmd *cmd)
{
	t_pipe_cmd	*pcmd;
	t_redir_cmd	*rcmd;
	t_exec_cmd	*ecmd;
	extern char	**environ;

	cmd_typecasting(cmd, &pcmd, &rcmd, &ecmd);
	if (!cmd)
		exit(EXIT_FAILURE);
	if (cmd->type == PIPE)
		set_pipe(pcmd);
	else if (cmd->type == EXEC)
	{
		if (ecmd->argv[0] == 0)
			exit(1);
		execve(ecmd->argv[0], ecmd->argv, environ);
		ft_putstr_fd(EXEC_ERR, 2);
		ft_putendl_fd(ecmd->argv[0], 2);
	}
	else if (cmd->type == REDIR)
		set_redirection(rcmd);
	exit(EXIT_SUCCESS);
}

// This function will set up the pipes, fork two child processes and
// call run_cmd in each child process to continue descending down the
// left and right trees respectively.
// The parent process will wait for both children to terminate.
// Even though fork is called twice, only two child processes are created
// because each child will exit when run_cmd is called.
void	set_pipe(t_pipe_cmd	*pcmd)
{
	int	pipefd[2];

	if (pipe(pipefd) < 0)
	{
		perror(PIPE_ERR);
		exit(EXIT_FAILURE);
	}
	if (fork_and_check() == 0)
	{
		dup2(pipefd[WRITE], STDOUT_FILENO);
		close_pipes(pipefd);
		run_cmd(pcmd->left);
	}
	if (fork_and_check() == 0)
	{
		dup2(pipefd[READ], STDIN_FILENO);
		close_pipes(pipefd);
		run_cmd(pcmd->right);
	}
	close_pipes(pipefd);
	wait(NULL);
	wait(NULL);
}

// This function will set up the correct redirection depending
// on whether it is a file or a heredoc. In both cases, it will
// call run_cmd to continue descending down the parsing tree.
void	set_redirection(t_redir_cmd *rcmd)
{
	if (rcmd->file)
		open_fd(rcmd);
	else if (rcmd->heredoc)
		pipe_heredoc(rcmd);
	run_cmd(rcmd->cmd);
}

// This function handles opening file descriptors for the REDIR nodes.
// If the third parameter in open() is omitted, file permissions default to
// the shell's umask (typically 022, which results in 0644 permissions). The
// more generous 0666 permissions will allow us to open the newly-created file.
// In a sequence of REDIR nodes, later redirections overwrite earlier ones
// because dup2 continuously reconfigures STDIN or STDOUT as we traverse
// down the chain.
void	open_fd(t_redir_cmd *rcmd)
{
	int	redir_fd;

	redir_fd = open(rcmd->file, rcmd->mode, 0666);
	if (redir_fd < 0)
	{
		ft_putstr_fd(FILE_OPEN_ERR, 2);
		ft_putendl_fd(rcmd->file, 2);
		exit(1);
	}
	dup2(redir_fd, rcmd->fd);
}

// This function will write the heredoc to a pipe so that it will
// have a file descriptor that can interact with the dup2 function.
// Since heredocs are typically temporary files, the heredoc string
// can be immediately freed after it has been written to the pipe for
// memory efficiency.
void	pipe_heredoc(t_redir_cmd *rcmd)
{
	int	pipefd[2];

	if (pipe(pipefd) < 0)
	{
		perror(PIPE_ERR);
		exit(EXIT_FAILURE);
	}
	write(pipefd[WRITE], rcmd->heredoc, ft_strlen(rcmd->heredoc));
	close(pipefd[WRITE]);
	dup2(pipefd[READ], STDIN_FILENO);
	close(pipefd[READ]);
	free(rcmd->heredoc);
	rcmd->heredoc = NULL;
}
