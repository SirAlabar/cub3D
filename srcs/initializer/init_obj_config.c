

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_config.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by your_login        #+#    #+#             */
/*   Updated: 2025/01/15 10:00:00 by your_login       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static void	init_pillar_config(t_object_config *config)
{
	config->texture_path = PILLAR_PATH;
	config->is_solid = true;
	config->collision_radius = 0.4;
	config->is_animated = false;
	config->num_frames = 1;
	config->frame_paths[0] = NULL;
	config->placement = PLACEMENT_AWAY;
}

static void	init_torch_config(t_object_config *config)
{
	config->texture_path = TORCH_PATH;
	config->is_solid = false;
	config->collision_radius = 0.2;
	config->is_animated = true;
	config->num_frames = 4;
	config->frame_paths[0] = TORCH_FRAME1;
	config->frame_paths[1] = TORCH_FRAME2;
	config->frame_paths[2] = TORCH_FRAME3;
	config->frame_paths[3] = TORCH_FRAME4;
	config->placement = PLACEMENT_WALL;
}

static void	init_barrel_config(t_object_config *config)
{
	config->texture_path = BARREL_PATH;
	config->is_solid = true;
	config->collision_radius = 0.35;
	config->is_animated = false;
	config->num_frames = 1;
	config->frame_paths[0] = NULL;
	config->placement = PLACEMENT_ANY;
}

static void	init_armor_config(t_object_config *config)
{
	config->texture_path = ARMOR_PATH;
	config->is_solid = true;
	config->collision_radius = 0.4;
	config->is_animated = false;
	config->num_frames = 1;
	config->frame_paths[0] = NULL;
	config->placement = PLACEMENT_WALL;
}

static void	init_plant_config(t_object_config *config)
{
	config->texture_path = PLANT_PATH;
	config->is_solid = false;
	config->collision_radius = 0.2;
	config->is_animated = false;
	config->num_frames = 1;
	config->frame_paths[0] = NULL;
	config->placement = PLACEMENT_WALL;
}

static void	init_lamp_config(t_object_config *config)
{
	config->texture_path = LAMP_PATH;
	config->is_solid = false;
	config->collision_radius = 0.2;
	config->is_animated = false;
	config->num_frames = 1;
	config->frame_paths[0] = NULL;
	config->placement = PLACEMENT_AWAY;
}

static void	init_crate_config(t_object_config *config)
{
	config->texture_path = CRATE_PATH;
	config->is_solid = true;
	config->collision_radius = 0.4;
	config->is_animated = false;
	config->num_frames = 1;
	config->frame_paths[0] = NULL;
	config->placement = PLACEMENT_ANY;
}

static void	init_statue_config(t_object_config *config)
{
	config->texture_path = STATUE_PATH;
	config->is_solid = true;
	config->collision_radius = 0.45;
	config->is_animated = false;
	config->num_frames = 1;
	config->frame_paths[0] = NULL;
	config->placement = PLACEMENT_ANY;
}

void	init_object_config(t_object_config *config, t_object_type type)
{
	if (!config)
		return ;
	ft_bzero(config, sizeof(t_object_config));
	if (type == OBJ_PILLAR)
		init_pillar_config(config);
	else if (type == OBJ_TORCH)
		init_torch_config(config);
	else if (type == OBJ_BARREL)
		init_barrel_config(config);
	else if (type == OBJ_ARMOR)
		init_armor_config(config);
	else if (type == OBJ_PLANT)
		init_plant_config(config);
	else if (type == OBJ_LAMP)
		init_lamp_config(config);
	else if (type == OBJ_CRATE)
		init_crate_config(config);
	else if (type == OBJ_STATUE)
		init_statue_config(config);
	else
		config->placement = PLACEMENT_ANY;
}
