/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/22 21:32:22 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

// Ative ou desative o debug
#define MOVEMENT_SOUND_DEBUG 1

// Tempo em ms de intervalo para log se não houver som tocado
// Isso evita spam no console quando não há mudanças
#define DEBUG_SILENT_INTERVAL 1000

void	debug_movement_sound(t_game *game, int is_moving, double current_time, 
							double last_sound_time)
{
	static double	last_debug_time = 0;
	static int		debug_counter = 0;
	
	if (!MOVEMENT_SOUND_DEBUG)
		return;
	
	// Se já passou muito tempo sem log, forçamos um log
	int force_log = (current_time - last_debug_time > DEBUG_SILENT_INTERVAL);
	
	// Verificações de estado
	int sound_valid = (game && game->sounds && game->sounds->footstep);
	
	// Nome da cor com base no estado
	const char *color_code;
	if (!sound_valid)
		color_code = "\033[31m"; // Vermelho para erro
	else if (is_moving && current_time - last_sound_time <= 350)
		color_code = "\033[33m"; // Amarelo para espera
	else if (is_moving)
		color_code = "\033[32m"; // Verde para som tocado
	else
		color_code = "\033[36m"; // Azul para parado
	
	// Só logamos se: estiver se movendo e pronto para tocar som, ou há erro, ou forçamos log
	if ((is_moving && current_time - last_sound_time > 350) || !sound_valid || force_log)
	{
		ft_printf("%s[MOVE_SND #%d] ", color_code, debug_counter++);
		
		// Status do movimento
		ft_printf("Movimento: %s | ", is_moving ? "SIM" : "NÃO");
		
		// Status do sistema de som
		if (!game)
			ft_printf("Game: NULL | ");
		else if (!game->sounds)
			ft_printf("Sounds: NULL | ");
		else if (!game->sounds->footstep)
			ft_printf("Footstep: NULL | ");
		
		// Status do timer
		if (is_moving)
			ft_printf("Intervalo: %.0fms | Último: %.0fms", 
				current_time - last_sound_time, last_sound_time);
		
		// Estado das teclas
		if (game)
			ft_printf(" | Teclas: W:%d S:%d A:%d D:%d", 
				game->p1.keys.w, game->p1.keys.s, 
				game->p1.keys.a, game->p1.keys.d);
		
		ft_printf("\033[0m\n");
		
		last_debug_time = current_time;
	}
}

// Versão de handle_movement com debug integrado
void	handle_movement(t_game *game)
{
	double		dir_x;
	double		dir_y;
	static double	last_footstep_time = 0;
	double		current_time;
	int			is_moving;
	
	dir_x = 0;
	dir_y = 0;
	
	// Parte original do cálculo de movimento
	if (game->p1.keys.w)
	{
		dir_x += game->p1.dir.x;
		dir_y += game->p1.dir.y;
	}
	if (game->p1.keys.s)
	{
		dir_x -= game->p1.dir.x;
		dir_y -= game->p1.dir.y;
	}
	keys_else(game, &dir_x, &dir_y);
	move_player(game, dir_x, dir_y);
	
	// Parte do som do movimento com debug
	is_moving = (game->p1.keys.w || game->p1.keys.s || 
				game->p1.keys.a || game->p1.keys.d);
	current_time = get_time_ms();
	
	// Debug antes de tocar o som
	debug_movement_sound(game, is_moving, current_time, last_footstep_time);
	
	// Reprodução do som de passos
	if (is_moving && game->sounds && game->sounds->footstep)
	{
		if (current_time - last_footstep_time > 350)
		{
			play_sound(game->sounds->footstep);
			last_footstep_time = current_time;
			
			// Debug após tocar o som (opcional)
			if (MOVEMENT_SOUND_DEBUG)
				ft_printf("\033[32m[MOVE_SND] Som tocado!\033[0m\n");
		}
	}
	
	// Parte original da rotação
	if (game->p1.keys.left)
		rotate_player(game, -game->p1.rot_speed);
	if (game->p1.keys.right)
		rotate_player(game, game->p1.rot_speed);
}

// void	handle_movement(t_game *game)
// {
// 	double	dir_x;
// 	double	dir_y;
// 	static double last_footstep_time = 0;
//     double current_time;

// 	dir_x = 0;
// 	dir_y = 0;
// 	if (game->p1.keys.w)
// 	{
// 		dir_x += game->p1.dir.x;
// 		dir_y += game->p1.dir.y;
// 	}
// 	if (game->p1.keys.s)
// 	{
// 		dir_x -= game->p1.dir.x;
// 		dir_y -= game->p1.dir.y;
// 	}
// 	keys_else(game, &dir_x, &dir_y);
// 	move_player(game, dir_x, dir_y);
// 	if ((game->p1.keys.w || game->p1.keys.s || game->p1.keys.a || game->p1.keys.d) 
// 	&& game->sounds && game->sounds->footstep)
// {
// 	current_time = get_time_ms();
	
// 	// Só tocar o som a cada 350ms (ajuste esse valor)
// 	if (current_time - last_footstep_time > 350)
// 	{
// 		play_sound(game->sounds->footstep);
// 		last_footstep_time = current_time;
// 	}
// }
// 	if (game->p1.keys.left)
// 		rotate_player(game, -game->p1.rot_speed);
// 	if (game->p1.keys.right)
// 		rotate_player(game, game->p1.rot_speed);
// }

int	key_press(int keycode, t_game *game)
{
	printf("Key pressed: %d\n", keycode);
	if (keycode == KEY_ESC)
		close_window(game);
	else if (keycode == KEY_E)
		interact_with_door(game);
	else if (keycode == KEY_W)
		game->p1.keys.w = 1;
	else if (keycode == KEY_S)
		game->p1.keys.s = 1;
	else if (keycode == KEY_D)
		game->p1.keys.d = 1;
	else if (keycode == KEY_A)
		game->p1.keys.a = 1;
	else if (keycode == KEY_LEFT)
		game->p1.keys.left = 1;
	else if (keycode == KEY_RIGHT)
		game->p1.keys.right = 1;
	else if (keycode == KEY_SPACE || keycode == MOUSE_LEFT)
		game->p1.is_firing = 1;
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (keycode == KEY_W)
		game->p1.keys.w = 0;
	else if (keycode == KEY_S)
		game->p1.keys.s = 0;
	else if (keycode == KEY_D)
		game->p1.keys.d = 0;
	else if (keycode == KEY_A)
		game->p1.keys.a = 0;
	else if (keycode == KEY_LEFT)
		game->p1.keys.left = 0;
	else if (keycode == KEY_RIGHT)
		game->p1.keys.right = 0;
	return (0);
}
