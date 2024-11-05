/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinti-a <abinti-a@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:04:01 by abinti-a          #+#    #+#             */
/*   Updated: 2024/11/05 14:47:03 by abinti-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

/**
 * An environment variable can only start with '_' or alphabet
 * After the first letter there can only be alphabet, digit or '_'
 */
static int	valid_var_name(const char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (name[0] != '_' && !ft_isalpha(name[0]))
		return (0);
	i = 1;
	while (name[i])
	{
		if (name[i] != '_' && !ft_isalnum(name[i]))
			return (0);
		i++;
	}
	return (1);
}

/**
 * Find the index of the environment variable needed to be removed
 */
static int	find_var_name(char **env, const char *name)
{
	int	i;
	int	name_len;

	name_len = (int)ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/**
 * Shift the remaining variable after the one we removed to the left
 */
static void	remove_var(char **env, int index)
{
	while (env[index])
	{
		env[index] = env[index + 1];
		index++;
	}
}

/**
 * Print error when variable name is not valid
 */
void	print_unset_error(char *arg)
{
	ft_putstr_fd("minishell: unset: '", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

/**
 * 1. Check if there are any arguments
 * 2. Iterate through argument and check for valid variable name
 * 3. Find index containing variable name in env
 * 4. If found, remove variable from env
 * 5. Return (0) if successful
 */
int	builtin_unset(char **args, char **env)
{
	int	i;
	int	exit_status;
	int	var_index;

	exit_status = 0;
	if (!args[1])
		return (1);
	i = 1;
	while (args[i])
	{
		if (!valid_var_name(args[i]))
		{
			print_unset_error(args[i]);
			exit_status = 1;
		}
		else
		{
			var_index = find_var_name(env, args[i]);
			if (var_index != -1)
				remove_var(env, var_index);
		}
		i++;
	}
	return (exit_status);
}
