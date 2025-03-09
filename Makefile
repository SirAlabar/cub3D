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

# **************************************************************************** #
#                                  COMPILATION                                   #
# **************************************************************************** #
CC      = cc
FLAGS   = -Wall -Wextra -Werror -g -Iincludes
IFLAGS  = -Iincludes/ -I${LIBFT_DIR}/src -I${MLX_DIR} -I${BASS_DIR}/includes
LIBFT   = ${LIBFT_DIR}/src/libft.a
MLX     = ${MLX_DIR}/libmlx.a
BASS    = ${BASS_DIR}/libbass.a

# Source files using wildcards
SRCS    = $(wildcard srcs/*.c) $(wildcard srcs/*/*.c) $(wildcard srcs/*/*/*.c)
OBJS    = ${SRCS:.c=.o}
INCLUDE = -Iincludes/ -I${LIBFT_DIR}/src -L${LIBFT_DIR}/src -I${MLX_DIR} ${MLXINC} -I${BASS_DIR}/includes

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
	SRCS := $(filter-out srcs/mlx/mlx_utils_mac.c, $(SRCS))
	
	# BASS configuration for Linux
	BASS_FLAGS = -L$(BASS_DIR) -lbass
	LINKFLAGS = -Wl,-rpath=./$(BASS_DIR)
else ifeq ($(UNAME_S),Darwin)
	MLX_DIR = libs/minilibx-mac-osx/
	MLXFLAGS = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
	FLAGS += -D MAC_OS -fsanitize=address
	SRCS := $(filter-out srcs/mlx/mlx_utils_linux.c, $(SRCS))
	
	# BASS configuration for macOS
	BASS_FLAGS = -L$(BASS_DIR) -lbass -framework CoreAudio -framework CoreFoundation
	LINKFIX = install_name_tool -change @loader_path/libbass.dylib @loader_path/$(BASS_DIR)/libbass.dylib $(NAME) && \
             install_name_tool -change libmlx.dylib @loader_path/$(MLX_DIR)/libmlx.dylib $(NAME)
endif

# **************************************************************************** #
#                                   RULES                                        #
# **************************************************************************** #

# Main rule
all: init $(NAME)

# Initialize by updating and compiling submodules
init: $(LIBFT) $(MLX) $(BASS_DIR)

# Bass initialization
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

${NAME}: ${OBJS} $(LIBFT) $(MLX) $(BASS_DIR)
	@${CC} ${FLAGS} ${IFLAGS} ${OBJS} ${INCLUDE} ${LIBFT} ${MLX} ${MLXFLAGS} ${BASS_FLAGS} ${LINKFLAGS} -o ${NAME}
	@if [ "$(UNAME_S)" = "Darwin" ]; then \
		install_name_tool -change @loader_path/libbass.dylib @loader_path/$(BASS_DIR)/libbass.dylib $(NAME) || true; \
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
	@echo "$(GREEN) Successfully compiled cub3D.$(RESET)"
	@echo

# Object compilation rule
.c.o:
	@${CC} ${FLAGS} ${IFLAGS} -c $< -o ${<:.c=.o}
	@clear
	@echo "$(RESET)[$(GREEN)OK$(RESET)]$(BLUE) Compiling $<$(YELLOW)"

# Clean rules
clean:
	@${RM} ${OBJS}
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
	@rm -rf ${LIBFT_DIR}
	@rm -rf ${MLX_DIR}
	@rm -rf ${BASS_DIR}
	@rm -f ${NAME}
	@if [ -d "${BASS_DIR}" ] && [ -f "${BASS_DIR}/Makefile" ]; then \
		${MAKE} --silent -C ${BASS_DIR} fclean; \
	fi
	@git submodule deinit -f --all 2>/dev/null || true
	@clear
	@echo
	@echo "$(RED)┏┓┓ ┏┓┏┓┳┓┏┓┳┓"
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

test: all
	@./$(NAME) maps/valid/valid1.cub

# Rebuild rule
re: fclean all

# Declare phony rules
.PHONY: all clean fclean re leak init