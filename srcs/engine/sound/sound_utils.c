/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourname <yourname@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:00:00 by yourname          #+#    #+#             */
/*   Updated: 2025/03/04 18:00:00 by yourname         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	clear_sounds(t_game *game)
{
	if (!game->sounds)
		return ;
	if (game->sounds->door)
		BASS_StreamFree(game->sounds->door);
	if (game->sounds->gun)
		BASS_StreamFree(game->sounds->gun);
	if (game->sounds->footstep)
		BASS_StreamFree(game->sounds->footstep);
	if (game->sounds->ambient)
		BASS_StreamFree(game->sounds->ambient);
	if (game->sounds->hurt)
		BASS_StreamFree(game->sounds->hurt);
	if (game->sounds->enemy)
		BASS_StreamFree(game->sounds->enemy);
	BASS_Free();
	free(game->sounds);
	game->sounds = NULL;
}

void	init_sound_empty(t_game *game)
{
	if (!game->sounds)
		return ;
	game->sounds->door = 0;
	game->sounds->gun = 0;
	game->sounds->footstep = 0;
	game->sounds->ambient = 0;
	game->sounds->hurt = 0;
	game->sounds->enemy = 0;
}

// int	init_sounds(t_game *game)
// {
// 	game->sounds = ft_calloc(1, sizeof(t_sounds));
// 	if (!game->sounds)
// 		return (0);
// 	init_sound_empty(game);
// 	if (!load_sound(&game->sounds->door, SOUND_FILE_DOOR))
// 		ft_printf("Warning: Failed to load door sound\n");
// 	if (!load_sound(&game->sounds->gun, SOUND_FILE_GUN))
// 		ft_printf("Warning: Failed to load gun sound\n");
// 	if (!load_sound(&game->sounds->footstep, SOUND_FILE_FOOTSTEP))
// 		ft_printf("Warning: Failed to load footstep sound\n");
// 	if (!load_sound(&game->sounds->ambient, SOUND_FILE_AMBIENT))
// 		ft_printf("Warning: Failed to load ambient sound\n");
// 	if (!load_sound(&game->sounds->hurt, SOUND_FILE_HURT))
// 		ft_printf("Warning: Failed to load hurt sound\n");
// 	if (!load_sound(&game->sounds->enemy, SOUND_FILE_ENEMY))
// 		ft_printf("Warning: Failed to load enemy sound\n");
// 	if (game->sounds->ambient)
// 		play_sound_loop(game->sounds->ambient, 1, 1);
// 	game->sounds->is_initialized = true;
// 	return (1);
// }

void debug_sound_status(t_game *game, const char *context)
{
	ft_printf("\033[33m[SOUND DEBUG] %s\033[0m\n", context);
	
	if (!game)
	{
		ft_printf("  - game: NULL\n");
		return;
	}
	
	ft_printf("  - game: %p\n", game);
	
	if (!game->sounds)
	{
		ft_printf("  - game->sounds: NULL\n");
		return;
	}
	
	ft_printf("  - game->sounds: %p\n", game->sounds);
	ft_printf("  - Initialized: %s\n", game->sounds->is_initialized ? "true" : "false");
	ft_printf("  - door: %u\n", game->sounds->door);
	ft_printf("  - gun: %u\n", game->sounds->gun);
	ft_printf("  - footstep: %u\n", game->sounds->footstep);
	ft_printf("  - ambient: %u\n", game->sounds->ambient);
	ft_printf("  - hurt: %u\n", game->sounds->hurt);
	ft_printf("  - enemy: %u\n", game->sounds->enemy);
}

int load_sound_with_debug(uint32_t *s, char *path)
{
	static int loaded;
	int error_code;
	
	ft_printf("\033[36m[LOAD_SOUND] Tentando carregar: %s\033[0m\n", path);
	
	if (!loaded)
	{
		ft_printf("  - Inicializando BASS...\n");
		loaded = BASS_Init(-1, 44100, 0, 0, 0);
		
		if (!loaded)
		{
			error_code = BASS_ErrorGetCode();
			ft_printf("  - \033[31mErro ao inicializar BASS: %d\033[0m\n", error_code);
			return (0);
		}
		ft_printf("  - BASS inicializado com sucesso\n");
	}
	
	if (*s)
	{
		ft_printf("  - Som já carregado: %u\n", *s);
		return (0);
	}
	
	*s = BASS_StreamCreateFile(FALSE, path, 0, 0, 0);
	
	if (!*s)
	{
		error_code = BASS_ErrorGetCode();
		ft_printf("  - \033[31mErro ao carregar arquivo: %d\033[0m\n", error_code);
		
		// Verificar se o arquivo existe
		int fd = open(path, O_RDONLY);
		if (fd < 0)
			ft_printf("  - \033[31mArquivo não encontrado: %s\033[0m\n", path);
		else
		{
			ft_printf("  - Arquivo existe mas não pôde ser carregado\n");
			close(fd);
		}
		return (0);
	}
	
	ft_printf("  - \033[32mSom carregado com sucesso: %u\033[0m\n", *s);
	return (1);
}

int init_sounds(t_game *game)
{
	ft_printf("\033[32m[INIT_SOUNDS] Iniciando...\033[0m\n");
	
	debug_sound_status(game, "Antes da alocação");
	
	game->sounds = ft_calloc(1, sizeof(t_sounds));
	if (!game->sounds)
	{
		ft_printf("\033[31m[INIT_SOUNDS] Falha na alocação de memória\033[0m\n");
		return (0);
	}
	
	ft_printf("[INIT_SOUNDS] Memória alocada: %p, tamanho: %lu bytes\n", 
		game->sounds, sizeof(t_sounds));
	
	init_sound_empty(game);
	
	debug_sound_status(game, "Após init_sound_empty");
	
	// Carregar os sons com debug
	if (!load_sound_with_debug(&game->sounds->door, SOUND_FILE_DOOR))
		ft_printf("Warning: Failed to load door sound\n");
	if (!load_sound_with_debug(&game->sounds->gun, SOUND_FILE_GUN))
		ft_printf("Warning: Failed to load gun sound\n");
	if (!load_sound_with_debug(&game->sounds->footstep, SOUND_FILE_FOOTSTEP))
		ft_printf("Warning: Failed to load footstep sound\n");
	if (!load_sound_with_debug(&game->sounds->ambient, SOUND_FILE_AMBIENT))
		ft_printf("Warning: Failed to load ambient sound\n");
	if (!load_sound_with_debug(&game->sounds->hurt, SOUND_FILE_HURT))
		ft_printf("Warning: Failed to load hurt sound\n");
	if (!load_sound_with_debug(&game->sounds->enemy, SOUND_FILE_ENEMY))
		ft_printf("Warning: Failed to load enemy sound\n");
	
	debug_sound_status(game, "Após carregar todos os sons");
	
	if (game->sounds->ambient)
	{
		ft_printf("[INIT_SOUNDS] Iniciando música ambiente...\n");
		play_sound_loop(game->sounds->ambient, 1, 1);
	}
	
	game->sounds->is_initialized = true;
	
	ft_printf("\033[32m[INIT_SOUNDS] Finalizado com sucesso\033[0m\n");
	return (1);
}