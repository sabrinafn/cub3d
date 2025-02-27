/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player_struct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:32:27 by sabrifer          #+#    #+#             */
/*   Updated: 2025/02/26 14:44:54 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub_3d.h"

t_player	*init_player_struct(void)
{
	t_player	*player;

	player = (t_player *)malloc(sizeof(t_player));

	player->pos_x = 1 + 0.5;
	player->pos_y = 1 + 0.5;
	//if (direction == 'N')
	player->dir_x = 0;
	player->dir_y = -1;
	player->plane_x = 0.66;
	player->plane_y = 0;

	return (player);
}

t_args	*init_map_struct(void)
{
	t_args	*map;

	map = (t_args *)malloc(sizeof(t_args));

	map->NO_wall = NULL;
	map->SO_wall = NULL;
	map->WE_wall = NULL;
	map->EA_wall = NULL;
	map->floor = NULL;
	map->ceiling = NULL;

	map->map = (char **)malloc(sizeof(char *) * 6);
	map->map[0] = ft_strdup("111111\n");
	map->map[1] = ft_strdup("100101\n");
	map->map[2] = ft_strdup("101001\n");
	map->map[3] = ft_strdup("1100N1\n");
	map->map[4] = ft_strdup("111111\n");
	map->map[5] = NULL;

	return (map);
}
