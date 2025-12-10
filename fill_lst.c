/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strieste <strieste@student.42.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 12:58:23 by strieste          #+#    #+#             */
/*   Updated: 2025/12/10 15:12:40 by strieste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	fill_lst(char **array)
{
	size_t	len;
	size_t	count;
	size_t	save;
	char	**cmd_array;

	len = 0;
	save = 0;
	while (array[len])
	{
		count = 0;
		if (array[len][0] == '|')
			len++;
		while (array[len] && array[len][0] != '|')
			len++;
		cmd_array = malloc((len + 1) * sizeof(char**));
		if (!cmd_array)
			return ;
		count = 0;
		if (array[save][0] == '|')
			save++;
		while (array[save] && array[save][0] != '|')
			cmd_array[count++] = array[save++];
		cmd_array[count] = 0;
		free(cmd_array);
	}
}

t_cmd	*fill_lst(char **array)
{
	size_t	len;
	size_t	count;
	char	**cmd_array;
	t_cmd	*cmd;
	t_cmd	*data;
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
