# **************************************************************************** #
#                                    COLORS                                      #
# **************************************************************************** #
RED    = $(shell printf "\33[31m")
GREEN  = $(shell printf "\33[32m")
WHITE  = $(shell printf "\33[37m")
YELLOW = $(shell printf "\33[33m")
RESET  = $(shell printf "\33[0m")
BLUE   = $(shell printf "\33[34m")
PURPLE = $(shell printf "\33[35m")
TITLE  = $(shell printf "\33[32;40m")

# **************************************************************************** #
#                                  DIRECTORIES                                   #
# **************************************************************************** #
LIBFT_DIR = libs/42-Libft
MLX_DIR   = libs/minilibx-linux/
BASS_DIR  = libs/42-Bass
NAME      = cub3D
NAME_BONUS = cub3D_bonus

# **************************************************************************** #
#                                  COMPILATION                                   #
# **************************************************************************** #
CC      = cc
FLAGS   = -Wall -Wextra -Werror -g
IFLAGS  = -Iincludes/ -I$(LIBFT_DIR)/src -I$(MLX_DIR)
IFLAGS_BONUS = -Iincludes/ -I$(LIBFT_DIR)/src -I$(MLX_DIR) -I$(BASS_DIR)/includes
LIBFT   = ${LIBFT_DIR}/src/libft.a
MLX     = ${MLX_DIR}/libmlx.a
BASS    = ${BASS_DIR}/libbass.a

# Source files
MAND_SRCS = mandatory/src/main.c \
            mandatory/src/checker/checker.c \
            mandatory/src/checker/flood_fill.c \
            mandatory/src/checker/flood_utils.c \
            mandatory/src/engine/double_buffer.c \
            mandatory/src/engine/engine_render.c \
            mandatory/src/engine/timer.c \
            mandatory/src/engine/draw/draw_basic.c \
            mandatory/src/engine/draw/draw_room.c \
            mandatory/src/engine/raycast/raycast_dda.c \
            mandatory/src/engine/raycast/raycast_utils.c \
            mandatory/src/engine/texture/texture_manager.c \
            mandatory/src/engine/texture/texture_clear.c \
            mandatory/src/engine/vector/vector_basic_operations.c \
            mandatory/src/engine/vector/vector_constructor.c \
            mandatory/src/engine/vector/vector_operations.c \
            mandatory/src/error/cleanup.c \
            mandatory/src/error/error.c \
            mandatory/src/events/key_handle.c \
            mandatory/src/events/mouse_move.c \
            mandatory/src/events/player_move.c \
            mandatory/src/events/player_move_utils.c \
            mandatory/src/initializer/init_colors.c \
            mandatory/src/initializer/init_game.c \
            mandatory/src/initializer/init_map.c \
            mandatory/src/initializer/init_map2.c \
            mandatory/src/initializer/init_player.c \
            mandatory/src/initializer/init_textures.c \
            mandatory/src/parse/validate_map.c


# Bonus source files
BONUS_SRCS = bonus/srcs/main.c \
             bonus/srcs/checker/checker.c \
             bonus/srcs/checker/flood_fill.c \
             bonus/srcs/checker/flood_utils.c \
             bonus/srcs/engine/double_buffer.c \
             bonus/srcs/engine/engine_render.c \
             bonus/srcs/engine/timer.c \
             bonus/srcs/engine/draw/draw_basic.c \
             bonus/srcs/engine/draw/draw_door.c \
             bonus/srcs/engine/draw/draw_floor_tex.c \
             bonus/srcs/engine/draw/draw_menu.c \
             bonus/srcs/engine/draw/draw_room.c \
             bonus/srcs/engine/draw/draw_skybox.c \
             bonus/srcs/engine/draw/draw_weapon.c \
             bonus/srcs/engine/draw/draw_weapon_utils.c \
			 bonus/srcs/engine/draw/scanline_rendering.c \
             bonus/srcs/engine/enemy/enemy.c \
			 bonus/srcs/engine/enemy/damage_feedback.c \
			 bonus/srcs/engine/enemy/damage_feedback2.c \
             bonus/srcs/engine/enemy/enemy_damage.c \
             bonus/srcs/engine/enemy/enemy_damage2.c \
             bonus/srcs/engine/enemy/enemy_dist.c \
             bonus/srcs/engine/enemy/enemy_dist2.c \
             bonus/srcs/engine/enemy/enemy_draw.c \
             bonus/srcs/engine/enemy/enemy_draw2.c \
             bonus/srcs/engine/enemy/enemy_shoot.c \
             bonus/srcs/engine/enemy/enemy_utils.c \
             bonus/srcs/engine/enemy/enemy_utils2.c \
             bonus/srcs/engine/enemy/enemy_utils3.c \
             bonus/srcs/engine/enemy/enemy_utils4.c \
             bonus/srcs/engine/health/health_bar.c \
             bonus/srcs/engine/minimap/minimap.c \
             bonus/srcs/engine/minimap/minimap_background.c \
             bonus/srcs/engine/minimap/minimap_draw.c \
             bonus/srcs/engine/minimap/minimap_player.c \
             bonus/srcs/engine/portals/portal_create.c \
             bonus/srcs/engine/portals/portal_detection.c \
             bonus/srcs/engine/portals/portal_gun.c \
             bonus/srcs/engine/portals/portal_init.c \
             bonus/srcs/engine/portals/portal_raycast.c \
             bonus/srcs/engine/portals/portal_render.c \
             bonus/srcs/engine/portals/portal_teleport.c \
             bonus/srcs/engine/portals/portal_update.c \
             bonus/srcs/engine/portals/portal_utils.c \
             bonus/srcs/engine/portals/portal_utils2.c \
             bonus/srcs/engine/raycast/raycast_dda.c \
             bonus/srcs/engine/raycast/raycast_utils.c \
             bonus/srcs/engine/sound/sound.c \
             bonus/srcs/engine/sound/sound_utils.c \
             bonus/srcs/engine/texture/texture_animation.c \
             bonus/srcs/engine/texture/texture_clear.c \
             bonus/srcs/engine/texture/texture_manager.c \
             bonus/srcs/engine/vector/vector_angular_operations.c \
             bonus/srcs/engine/vector/vector_basic_operations.c \
             bonus/srcs/engine/vector/vector_constructor.c \
             bonus/srcs/engine/vector/vector_int_operations.c \
             bonus/srcs/engine/vector/vector_operations.c \
             bonus/srcs/error/cleanup.c \
             bonus/srcs/error/cleanup_img.c \
             bonus/srcs/error/cleanup_menu.c \
             bonus/srcs/error/cleanup_sounds.c \
             bonus/srcs/error/error.c \
             bonus/srcs/events/door_interact.c \
             bonus/srcs/events/door_utils.c \
             bonus/srcs/events/key_handle.c \
             bonus/srcs/events/key_handle2.c \
             bonus/srcs/events/mouse_move.c \
             bonus/srcs/events/player_move.c \
             bonus/srcs/events/player_move_utils.c \
             bonus/srcs/events/utils.c \
             bonus/srcs/initializer/init_colors.c \
             bonus/srcs/initializer/init_colors2.c \
             bonus/srcs/initializer/init_doors.c \
             bonus/srcs/initializer/init_floor.c \
             bonus/srcs/initializer/init_game.c \
             bonus/srcs/initializer/init_map.c \
             bonus/srcs/initializer/init_map2.c \
             bonus/srcs/initializer/init_menu_textures.c \
             bonus/srcs/initializer/init_menu_textures2.c \
             bonus/srcs/initializer/init_player.c \
             bonus/srcs/initializer/init_skybox.c \
             bonus/srcs/initializer/init_textures.c \
             bonus/srcs/initializer/menu.c \
             bonus/srcs/initializer/menu_animation.c \
             bonus/srcs/initializer/printers.c \
             bonus/srcs/initializer/read_error.c \
             bonus/srcs/parse/validate_map.c

INCLUDE = -Iincludes/ -I${LIBFT_DIR}/src -L${LIBFT_DIR}/src -I${MLX_DIR} ${MLXINC}

INCLUDE_BONUS = -Iincludes/ -I${LIBFT_DIR}/src -L${LIBFT_DIR}/src -I${MLX_DIR} ${MLXINC} -I${BASS_DIR}/includes

MAND_OBJS = ${MAND_SRCS:.c=.o}
BONUS_OBJS = ${BONUS_SRCS:.c=.o}

# Debug tools
VALGRIND = valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all

# **************************************************************************** #
#                                OS DETECTION                                    #
# **************************************************************************** #
# OS Detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	MLX_DIR = libs/minilibx-linux/
	MLXFLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lbsd
	MLXINC = -I/usr/include
	
	# BASS configuration for Linux
	BASS_FLAGS = -L$(BASS_DIR) -lbass
	LINKFLAGS = -Wl,-rpath=./$(BASS_DIR)
else ifeq ($(UNAME_S),Darwin)
	MLX_DIR = libs/minilibx-mac-osx/
	MLXFLAGS = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
	FLAGS += -D MAC_OS -fsanitize=address
	
	# BASS configuration for macOS
	BASS_FLAGS = -L$(BASS_DIR) -lbass -framework CoreAudio -framework CoreFoundation
	LINKFIX = install_name_tool -change @loader_path/libbass.dylib @loader_path/$(BASS_DIR)/libbass.dylib $(NAME) && \
             install_name_tool -change libmlx.dylib @loader_path/$(MLX_DIR)/libmlx.dylib $(NAME)
endif

# Adjust MLX paths for mandatory vs bonus based on OS
ifeq ($(UNAME_S),Linux)
	MAND_SRCS += mandatory/src/mlx/mlx_utils_linux.c
	BONUS_SRCS += bonus/srcs/mlx/mlx_utils_linux.c
else ifeq ($(UNAME_S),Darwin)
	MAND_SRCS += mandatory/src/mlx/mlx_utils_mac.c
	BONUS_SRCS += bonus/srcs/mlx/mlx_utils_mac.c
endif

# **************************************************************************** #
#                                   RULES                                        #
# **************************************************************************** #

# Main rule
all: init_mandatory $(NAME)

# Bonus rule
bonus: init_bonus $(NAME_BONUS)

# Initialize mandatory by updating and compiling required submodules
init_mandatory: $(LIBFT) $(MLX)

# Initialize bonus by updating and compiling all submodules including BASS
init_bonus: $(LIBFT) $(MLX) $(BASS_DIR)

# Bass initialization (only for bonus)
$(BASS_DIR):
	@echo "$(YELLOW)Initializing BASS library...$(RESET)"
	@if [ ! -d "$(BASS_DIR)" ]; then \
		git clone --quiet https://github.com/SirAlabar/42-Bass.git $(BASS_DIR); \
	else \
		git -C $(BASS_DIR) pull --quiet; \
	fi
	@if [ -f "$(BASS_DIR)/Makefile" ]; then \
		$(MAKE) --silent -C $(BASS_DIR); \
	fi
	@mkdir -p includes
	@if [ -f "$(BASS_DIR)/includes/bass.h" ]; then \
		cp -f $(BASS_DIR)/includes/bass.h includes/; \
	fi
	@echo "$(GREEN)BASS library setup complete$(RESET)"

# Libft initialization
$(LIBFT):
	@echo "$(YELLOW)Initializing Libft...$(RESET)"
	@git submodule update --init --recursive $(LIBFT_DIR)
	@$(MAKE) --silent -C $(LIBFT_DIR)/src

# MLX initialization with OS detection
$(MLX):
	@echo "$(YELLOW)Initializing MinilibX...$(RESET)"
	@rm -rf $(MLX_DIR)
	@if [ "$(UNAME_S)" = "Darwin" ]; then \
		git clone --quiet https://github.com/dannywillems/minilibx-mac-osx.git $(MLX_DIR); \
	else \
		git clone --quiet https://github.com/42paris/minilibx-linux.git $(MLX_DIR); \
	fi
	@cd $(MLX_DIR) && ./configure > /dev/null 2>&1
	@$(MAKE) --silent -C $(MLX_DIR) > /dev/null 2>&1

# Compilation rule for mandatory
${NAME}: ${MAND_OBJS} $(LIBFT) $(MLX)
	@${CC} ${FLAGS} ${IFLAGS} ${MAND_OBJS} ${INCLUDE} ${LIBFT} ${MLX} ${MLXFLAGS} -o ${NAME}
	@if [ "$(UNAME_S)" = "Darwin" ]; then \
		install_name_tool -change libmlx.dylib @loader_path/$(MLX_DIR)/libmlx.dylib $(NAME) || true; \
	fi
	@echo "$(TITLE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "$(PURPLE) ██████╗ ██╗   ██╗██████╗ ██████╗ ██████╗ "
	@echo " ██║     ██║   ██║██╔══██╗╚════██╗██╔══██╗"
	@echo " ██║     ██║   ██║██████╔╝ █████╔╝██║  ██║"
	@echo " ██║     ██║   ██║██╔══██╗ ╚═══██╗██║  ██║"
	@echo " ██████╔╝╚██████╔╝██████╔╝██████╔╝██████╔╝"
	@echo " ╚═════╝  ╚═════╝ ╚═════╝ ╚═════╝ ╚═════╝ "
	@echo "$(GREEN)━━━━━━━━━━━━━━━━━[$(RESET)Made with $(RED)♥ $(RESET)by $(PURPLE)@marsoare$(RESET) and $(PURPLE)@hluiz-ma$(TITLE)]━━━━━━"
	@echo
	@echo "$(GREEN) Successfully compiled cub3D (mandatory).$(RESET)"
	@echo

# Compilation rule for bonus
${NAME_BONUS}: ${BONUS_OBJS} $(LIBFT) $(MLX) $(BASS_DIR)
	@${CC} ${FLAGS} ${IFLAGS_BONUS} ${BONUS_OBJS} ${INCLUDE_BONUS} ${LIBFT} ${MLX} ${MLXFLAGS} ${BASS_FLAGS} ${LINKFLAGS} -o ${NAME_BONUS}
	@if [ "$(UNAME_S)" = "Darwin" ]; then \
		install_name_tool -change @loader_path/libbass.dylib @loader_path/$(BASS_DIR)/libbass.dylib $(NAME_BONUS) || true; \
		install_name_tool -change libmlx.dylib @loader_path/$(MLX_DIR)/libmlx.dylib $(NAME_BONUS) || true; \
	fi
	@echo "$(TITLE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "$(PURPLE) ██████╗ ██╗   ██╗██████╗ ██████╗ ██████╗ "
	@echo " ██║     ██║   ██║██╔══██╗╚════██╗██╔══██╗"
	@echo " ██║     ██║   ██║██████╔╝ █████╔╝██║  ██║"
	@echo " ██║     ██║   ██║██╔══██╗ ╚═══██╗██║  ██║"
	@echo " ██████╔╝╚██████╔╝██████╔╝██████╔╝██████╔╝"
	@echo " ╚═════╝  ╚═════╝ ╚═════╝ ╚═════╝ ╚═════╝ "
	@echo "$(GREEN)━━━━━━━━━━━━━━━━━[$(RESET)Made with $(RED)♥ $(RESET)by $(PURPLE)@marsoare$(RESET) and $(PURPLE)@hluiz-ma$(TITLE)]━━━━━━"
	@echo
	@echo "$(GREEN) Successfully compiled cub3D (bonus).$(RESET)"
	@echo

# Object compilation rule for mandatory
mandatory/src/%.o: mandatory/src/%.c
	@${CC} ${FLAGS} ${IFLAGS} -c $< -o $@
	@echo "$(RESET)[$(GREEN)OK$(RESET)]$(BLUE) Compiling $<$(YELLOW)"

# Object compilation rule for bonus
bonus/srcs/%.o: bonus/srcs/%.c
	@${CC} ${FLAGS} ${IFLAGS_BONUS} -c $< -o $@
	@echo "$(RESET)[$(GREEN)OK$(RESET)]$(BLUE) Compiling $<$(YELLOW)"

# Clean rules
clean:
	@${RM} ${MAND_OBJS} ${BONUS_OBJS}
	@if [ -d "${LIBFT_DIR}/src" ]; then \
		${MAKE} --silent -C ${LIBFT_DIR}/src clean; \
	fi
	@if [ -d "${BASS_DIR}" ] && [ -f "${BASS_DIR}/Makefile" ]; then \
		${MAKE} --silent -C ${BASS_DIR} clean; \
	fi
	@clear
	@echo
	@echo "$(RED)┏┓┓ ┏┓┏┓┳┓┏┓┳┓"
	@echo "┃ ┃ ┣ ┣┫┃┃┣ ┃┃"
	@echo "┗┛┗┛┗┛┛┗┛┗┗┛┻┛"
	@echo

fclean: clean
	@rm -f ${NAME} ${NAME_BONUS}
	@if [ -d "${LIBFT_DIR}" ] && [ -f "${LIBFT_DIR}/src/Makefile" ]; then \
		${MAKE} --silent -C ${LIBFT_DIR}/src fclean; \
	fi
	@if [ -d "${MLX_DIR}" ] && [ -f "${MLX_DIR}/Makefile" ]; then \
		${MAKE} --silent -C ${MLX_DIR} clean; \
	fi
	@if [ -d "${BASS_DIR}" ] && [ -f "${BASS_DIR}/Makefile" ]; then \
		${MAKE} --silent -C ${BASS_DIR} fclean; \
	fi
	@clear
	@echo
	@echo "$(RED)┏┓┓ ┏┓┏┓┳┓┏┓┳┓"
	@echo "┃ ┃ ┣ ┣┫┃┃┣ ┃┃"
	@echo "┗┛┗┛┗┛┛┗┛┗┗┛┻┛"
	@echo

deep_clean: fclean
	@rm -rf ${LIBFT_DIR}
	@rm -rf ${MLX_DIR}
	@rm -rf ${BASS_DIR}
	@git submodule deinit -f --all 2>/dev/null || true
	@clear
	@echo
	@echo "$(RED)┏┓┓ ┏┓┏┓┳┓┏┓┳┓ (DEEP CLEAN)"
	@echo "┃ ┃ ┣ ┣┫┃┃┣ ┃┃"
	@echo "┗┛┗┛┗┛┛┗┛┗┗┛┻┛"
	@echo

# Memory leak check based on OS
leak: ${NAME}
ifeq ($(UNAME_S),Linux)
	@echo "$(YELLOW)Running Valgrind for leak check...$(RESET)"
	@$(VALGRIND) ./${NAME} maps/valid/valid1.cub
else ifeq ($(UNAME_S),Darwin)
	@echo "$(YELLOW)Running leak check for macOS...$(RESET)"
	@leaks --atExit -- ./${NAME} maps/valid/valid1.cub
endif

leak_bonus: ${NAME_BONUS}
ifeq ($(UNAME_S),Linux)
	@echo "$(YELLOW)Running Valgrind for leak check (bonus)...$(RESET)"
	@$(VALGRIND) ./${NAME_BONUS} maps/valid/valid1.cub
else ifeq ($(UNAME_S),Darwin)
	@echo "$(YELLOW)Running leak check for macOS (bonus)...$(RESET)"
	@leaks --atExit -- ./${NAME_BONUS} maps/valid/valid1.cub
endif

test: all
	@./$(NAME) maps/valid/valid1.cub

test_bonus: bonus
	@./$(NAME_BONUS) maps/valid/valid1.cub

# Rebuild rule
re: fclean all

# Rebuild bonus rule
re_bonus: fclean bonus

# Declare phony rules
.PHONY: all bonus clean fclean deep_clean re re_bonus leak leak_bonus test test_bonus init_mandatory init_bonus