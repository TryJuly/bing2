/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strieste <strieste@student.42.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 12:58:23 by strieste          #+#    #+#             */
/*   Updated: 2025/12/10 19:28:12 by strieste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	count_args(char **array, int len);

t_cmd	*fill_lst(char **array)
{
	size_t	len;
	t_cmd	*lst;
	t_cmd	*current;

	len = 0;
	lst = NULL;
	while (array[len])
	{
		current->args = malloc((count_args(array, len) + 1) * sizeof(char**));
		if (!current->args)
			return (printf("%sError malloc fill_lst%s\n", RED, NC), NULL);
		while (array[len] && array[len] != '|')
		{
			if (is_redir(array[len]))
			{
				add_redir_node(array[len], array[len + 1], current);
				len += 2;
			}
			else
				add_args_node(array[len++], current);
		}
		add_back(lst, current);
		if (array[len][0] == '|' && array[len + 1])
			len++;
	}
	return (lst);
}

static int	is_redir(char *str)
{
	if (str[0] == '>' || str[0] == '<')
		return (1);
	if (str[0] == '>' && str[1] == '>')
		return (1);
	if (str[0] == '<' && str[1] == '<')
		return (1);
	return (0);
}

static int	count_args(char **array, int len)
{
	size_t	count;

	count = 0;
	while (array[len] && array[len][0] != '|')
	{
		if (array[len][0] == '<' || array[len][0] == '<'
			|| array[len][0] == '>' && array[len][0] == '>'
			|| array[len][0] == '<' && array[len][0] == '<')
		{
			if (array[len + 1])
				len += 2;
			else
				return (-1);
		}
		else
		{
			len++;
			count++;
		}
	}
	return (count);
}

t_cmd	*new_node(char **cmd)
{
	t_cmd	*new;
	size_t	len;
}

void	add_back(t_cmd *cmd, t_cmd *new)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return ;
}

// void	fill_lst(char **array)
// {
// 	size_t	len;
// 	size_t	count;
// 	size_t	save;
// 	char	**cmd_array;

// 	len = 0;
// 	save = 0;
// 	while (array[len])
// 	{
// 		count = 0;
// 		if (array[len][0] == '|')
// 			len++;
// 		while (array[len] && array[len][0] != '|')
// 			len++;
// 		cmd_array = malloc((len + 1) * sizeof(char**));
// 		if (!cmd_array)
// 			return ;
// 		count = 0;
// 		if (array[save][0] == '|')
// 			save++;
// 		while (array[save] && array[save][0] != '|')
// 			cmd_array[count++] = array[save++];
// 		cmd_array[count] = 0;
// 		free(cmd_array);
// 	}
// }
