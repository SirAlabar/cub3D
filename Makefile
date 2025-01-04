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
NAME      = cub3d

# **************************************************************************** #
#                                  COMPILATION                                   #
# **************************************************************************** #
CC      = cc
FLAGS   = -Wall -Wextra -Werror -g -Iincludes
IFLAGS  = -Iincludes/ -I${LIBFT_DIR}/src -I${MLX_DIR}
LIBFT   = ${LIBFT_DIR}/src/libft.a
MLX     = ${MLX_DIR}/libmlx.a

# Source files using wildcards
SRCS    = $(wildcard srcs/*.c) $(wildcard srcs/*/*.c)
OBJS    = ${SRCS:.c=.o}
INCLUDE = -Iincludes/ -I${LIBFT_DIR}/src -L${LIBFT_DIR}/src -I${MLX_DIR} ${MLXINC}

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
else ifeq ($(UNAME_S),Darwin)
	MLX_DIR = libs/minilibx-mac-osx/
	MLXFLAGS = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
	FLAGS += -D MAC_OS -fsanitize=address
	SRCS := $(filter-out srcs/mlx/mlx_utils_linux.c, $(SRCS))
endif

# **************************************************************************** #
#                                   RULES                                        #
# **************************************************************************** #

# Main rule
all: init $(NAME)

# Initialize by updating and compiling submodules
init: $(LIBFT) $(MLX)

# Libft initialization
$(LIBFT):
	@echo "$(YELLOW)Initializing Libft...$(RESET)"
	@git submodule update --init --recursive $(LIBFT_DIR)
	@$(MAKE) --silent -C $(LIBFT_DIR)/src

# MLX initialization with OS detection
$(MLX):
$(MLX):
	@echo "$(YELLOW)Initializing MinilibX...$(RESET)"
	@rm -rf $(MLX_DIR)
	@if [ "$(UNAME_S)" = "Darwin" ]; then \
		git clone https://github.com/dannywillems/minilibx-mac-osx.git $(MLX_DIR); \
	else \
		git clone https://github.com/42paris/minilibx-linux.git $(MLX_DIR); \
	fi
	@$(MAKE) -C $(MLX_DIR)

# Compilation rule
${NAME}: ${OBJS}
	@${CC} ${FLAGS} ${IFLAGS} ${OBJS} ${INCLUDE} ${LIBFT} ${MLX} ${MLXFLAGS} -o ${NAME}
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
	@cd ${LIBFT_DIR}/src && $(MAKE) --silent clean
	@clear
	@echo
	@echo "$(RED)┏┓┓ ┏┓┏┓┳┓┏┓┳┓"
	@echo "┃ ┃ ┣ ┣┫┃┃┣ ┃┃"
	@echo "┗┛┗┛┗┛┛┗┛┗┗┛┻┛"
	@echo

fclean: clean
	@rm -rf ${LIBFT_DIR}
	@rm -rf ${MLX_DIR}
	@rm -f ${NAME}
	@git submodule deinit -f --all
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

# Rebuild rule
re: fclean all

# Declare phony rules
.PHONY: all clean fclean re leak init