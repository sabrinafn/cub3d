/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_info_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgonzaga <mgonzaga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:23:25 by mgonzaga          #+#    #+#             */
/*   Updated: 2025/03/25 16:20:45 by mgonzaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub_3d.h"

int	get_map(char **matrix, t_content *s_content)
{
	int	i;
	int	l;

	i = 0;
	l = get_map_sizes_y(matrix);
	s_content->map = ft_calloc((l + 2) , sizeof(char *));
	if (!s_content->map)
		return (print_error(ERROR_3));
	l = find_map(matrix);
	while (matrix[l] != NULL)
	{
		s_content->map[i] = get_map_string(get_map_sizes_x(matrix), matrix[l]);
		if (!s_content->map[i])
		{
			free_matrix(s_content->map);
			return (print_error(ERROR_3));
		}
		l++;
		i++;
	}
	s_content->map[i] = NULL;
	return (1);
}

char *get_map_string(int size_x, char *string)
{
	char *result;
	int i;
	int j;
	
	i = 0;
	j = 0;
	printf("%i sizex \n", size_x);
	result = ft_calloc((size_x + 1) , sizeof(char));
	if (!result)
		return (NULL);
	while(string[i] != '\n' && string[i] != '\0')
	{
		result[j] = string[i];
		j++;
		i++;
	}
	while(j < size_x)
	{
		result[j] = 'j';
		j++;
	}
	result[j] = '\0';
	return(result);
}

int	get_map_sizes_y(char **file_matrix)
{
	int	i;
	int	count;
	int lines;

	i = find_map(file_matrix);
	lines = 0;
	count = 0;
	while (file_matrix[i] != NULL)
	{
		while (file_matrix[i][count] == ' ')
			count++;
		if (file_matrix[i][count] == '\n'
			|| file_matrix[i][count] == '\0')
			break ;
		i++;
		lines++;
	}
	printf(" numero de y %i\n", lines);
	return(lines);
}

int	get_map_sizes_x(char **file_matrix)
{
	int	i;
	int	size_line;
	int	comp_size;

	i = find_map(file_matrix);
	size_line = 0;
	comp_size = 0;
	size_line = ft_strlen(file_matrix[i]);
	while (file_matrix[i] != NULL)
	{
		comp_size = ft_strlen(file_matrix[i]);
		if (comp_size > size_line)
			size_line = comp_size;
		i++;
	}
	return(size_line);
}
