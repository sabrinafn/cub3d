/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:49:43 by sabrifer          #+#    #+#             */
/*   Updated: 2025/02/27 17:38:00 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub_3d.h"

// RGB 244, 194, 194
// # define WIDTH 800
// # define HEIGHT 601

#define RGB_Red 0xFF0000FF    // Red
#define RGB_Green 0x00FF00FF  // Green
#define RGB_Blue 0x0000FFFF   // Blue
#define RGB_White 0xFFFFFFFF  // White
#define RGB_Yellow 0xFFFF00FF // Yellow

int	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

mlx_image_t	*init_img(mlx_t *mlx, unsigned int floor, unsigned int ceiling)
{
	mlx_image_t	*img;
	int			i;
	int			j;

	i = 0;
	j = 0;
	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_putstr_fd("Error creating image\n", 2);
	while (i < WIDTH)
	{
		j = 0;
		while (j < HEIGHT)
		{
			if (j < HEIGHT / 2)
				mlx_put_pixel(img, i, j, ceiling);
			else
				mlx_put_pixel(img, i, j, floor);
			j++;
		}
		i++;
	}
	return (img);
}

void	calculate_rays_direction(t_player *player, t_args *map,
		mlx_image_t *img)
{
	int			x;
	t_ray		*ray;
	uint32_t	color;
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	uint8_t		a;
	int			i;
	int			hit;

	ray = (t_ray *)malloc(sizeof(t_ray));
	x = 0;
	while (x < WIDTH)
	{
		ray->camera_x = 2 * x / (double)WIDTH - 1;
		ray->dir_x = player->dir_x + (player->plane_x * ray->camera_x);
		ray->dir_y = player->dir_y + (player->plane_y * ray->camera_x);

		// which box of the map we're in
		ray->map_x = (int)player->pos_x;
		ray->map_y = (int)player->pos_y;

		// length of ray from current position to next x or y-side
		if (ray->dir_x == 0)
		ray->delta_dist_x = 1e30;
		else
			ray->delta_dist_x = fabs(1 / ray->dir_x);
		if (ray->dir_y == 0)
			ray->delta_dist_y = 1e30;
		else
			ray->delta_dist_y = fabs(1 / ray->dir_y);

		// double perpWallDist;
		// what direction to step in x or y-direction (either +1 or -1)
		// int stepX;
		// int stepY;

		hit = 0; // has it found a wall?

		// calculate step and initial sideDist
		if (ray->dir_x < 0)
		{
			ray->step_x = -1;
			ray->side_dist_x = (player->pos_x - ray->map_x) * ray->delta_dist_x;
		}
		else
		{
			ray->step_x = 1;
			ray->side_dist_x = (ray->map_x + 1.0 - player->pos_x)
				* ray->delta_dist_x;
		}

		if (ray->dir_y < 0)
		{
			ray->step_y = -1;
			ray->side_dist_y = (player->pos_y - ray->map_y) * ray->delta_dist_y;
		}
		else
		{
			ray->step_y = 1;
			ray->side_dist_y = (ray->map_y + 1.0 - player->pos_y)
				* ray->delta_dist_y;
		}

		// perform DDA
		while (hit == 0)
		{
			// jump to next map square, either in x-direction, or in y-direction
			if (ray->side_dist_x <= ray->side_dist_y)
			{
				ray->side_dist_x += ray->delta_dist_x;
				ray->map_x += ray->step_x;
				ray->side = 0;
			}
			else
			{
				ray->side_dist_y += ray->delta_dist_y;
				ray->map_y += ray->step_y;
				ray->side = 1;
			}
			// Check if ray has hit a wall
			// this part needs a proper function to check whether the pixel
			// has touched a char that is 1 or the player itself.
			if (map->map[ray->map_y][ray->map_x] == '1'
				&& ray->map_x < map->map_max_x && ray->map_y < map->map_max_y)
				hit = 1;

		}

		// Calculate distance projected on camera direction
		if (ray->side == 0)
			ray->perp_wall_dist = (ray->side_dist_x - ray->delta_dist_x);
		else
			ray->perp_wall_dist = (ray->side_dist_y - ray->delta_dist_y);

		// Calculate height of line to draw on screen
		ray->line_height = (int)(HEIGHT / ray->perp_wall_dist);
		
		// calculate lowest and highest pixel to fill in current stripe
		ray->draw_start = -ray->line_height / 2 + (HEIGHT / 2);
		if (ray->draw_start < 0)
			ray->draw_start = 0;


		ray->draw_end = (ray->line_height / 2) + (HEIGHT / 2);
		if (ray->draw_end >= HEIGHT)
			ray->draw_end = HEIGHT - 1;
		
		color = RGB_Green;
		
		if (ray->side == 1)
		{
			// Darken the color (divide each component by 2)
			r = (color >> 24) & 0xFF;
			g = (color >> 16) & 0xFF;
			b = (color >> 8) & 0xFF;
			a = color & 0xFF;
			r /= 2;
			g /= 2;
			b /= 2;
			color = (r << 24) | (g << 16) | (b << 8) | a;
		}
		i = ray->draw_start;
		while (i < ray->draw_end)
		{
			if (i >= 0 && i < HEIGHT && x >= 0 && x < WIDTH)
				mlx_put_pixel(img, x, i, color);
			i++;
		}
	x++;
	}
}

void	key_pressed_function(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = param;
	(void)keydata;

	// W 
	// A
	// S
	// D
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	printf("after first if\n");
	//if (mlx_is_key_down(game->mlx, MLX_KEY_W))
	//	move_player_w(game);
}

void	move_player_w(t_game *game)
{
	printf("move_player_w\n");
	int	new_pos_x;
	int	new_pos_y;

	new_pos_x = game->player_struct->pos_x + game->ray_struct->dir_x * MOVE_SPEED;
	new_pos_y = game->player_struct->pos_y + game->ray_struct->dir_y * MOVE_SPEED;

	if (game->map_struct->map[new_pos_y][new_pos_x] != '1'
		&& new_pos_x < game->map_struct->map_max_x
		&& new_pos_y < game->map_struct->map_max_y)
	{
		printf("inside if statement\n");
		game->player_struct->pos_x = new_pos_x;
		game->player_struct->pos_y = new_pos_y;
		calculate_rays_direction(game->player_struct, game->map_struct, game->img);
	}
}

void	init_window(t_game *game)
{
	unsigned int	ceiling;
	unsigned int	floor;

	ceiling = get_rgba(255, 199, 231, 255);
	floor = get_rgba(128, 112, 214, 255);
	game->mlx = mlx_init(WIDTH, HEIGHT, "Cub3D", false);
	if (!game->mlx)
		ft_putstr_fd("Error opening window\n", 2);
	game->img = init_img(game->mlx, ceiling, floor);
	mlx_image_to_window(game->mlx, game->img, WIDTH, HEIGHT);
	calculate_rays_direction(game->player_struct, game->map_struct, game->img);
	mlx_image_to_window(game->mlx, game->img, 0, 0);
	mlx_key_hook(game->mlx, &key_pressed_function, game);
	mlx_loop(game->mlx);
}
/********************************************************************
Implement Raycasting Loop:

- Create a function that iterates through each vertical screen column.
- For each column:
	- Calculate the ray direction.
	- Call the DDA function to find the wall intersection.
	- Calculate the distance to the wall.
	- Calculate the wall height on the screen.
	- Calculate the texture coordinate.
********************************************************************/
