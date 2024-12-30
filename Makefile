# COLORS
RED    = $(shell printf "\33[31m")
GREEN  = $(shell printf "\33[32m")
WHITE  = $(shell printf "\33[37m")
YELLOW = $(shell printf "\33[33m")
RESET  = $(shell printf "\33[0m")
BLUE   = $(shell printf "\33[34m")
PURPLE = $(shell printf "\33[35m")
TITLE  = $(shell printf "\33[32;40m")

LIBFT_DIR = libs/42-Libft
MLX_DIR  = libs/minilibx-linux/
NAME     = cub3d
FLAGS    = -Wall -Wextra -Werror -g -Iincludes -fsanitize=address
IFLAGS   = -Iincludes/ -I${LIBFT_DIR}/src -I${MLX_DIR}
MLXFLAGS = -Lmlx -lmlx -framework OpenGL -framework AppKit -lbsd
LIBFT    = ${LIBFT_DIR}/src/libft.a
MLX      = ${MLX_DIR}/libmlx.a
CC       = cc
SRCS     = $(wildcard srcs/*.c) $(wildcard srcs/*/*.c)
OBJS     = ${SRCS:.c=.o}
INCLUDE  = -Iincludes/ -I${LIBFT_DIR}/src -L${LIBFT_DIR}/src -I${MLX_DIR} ${MLXINC}
VALGRIND = valgrind  --track-fds=yes --leak-check=full --show-leak-kinds=all

# Detect operating system
UNAME_S := $(shell uname -s)

# Set MLX directories and flags based on OS
ifeq ($(UNAME_S),Linux)
    MLX_DIR = libs/minilibx-linux/
    MLXFLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lbsd
    MLXINC = -I/usr/include
else ifeq ($(UNAME_S),Darwin)
    MLX_DIR = libs/minilibx-mac-osx/
    MLXFLAGS = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
    FLAGS    = -Wall -Wextra -Werror -g -Iincludes -D MAC_OS -fsanitize=address
endif

all: init $(NAME)

# Initialize by updating and compiling submodules
init: $(LIBFT) $(MLX)

$(LIBFT):
	@echo "$(YELLOW)Initializing Libft...$(RESET)"
	@git submodule update --init --recursive $(LIBFT_DIR)
	@$(MAKE) --silent -C $(LIBFT_DIR)/src

# Update the MLX initialization rule
$(MLX):
	@echo "$(YELLOW)Initializing MinilibX...$(RESET)"
	@if [ "$(UNAME_S)" = "Darwin" ]; then \
		git submodule add -f https://github.com/dannywillems/minilibx-mac-osx.git $(MLX_DIR) 2>/dev/null || true; \
	else \
		git submodule add -f https://github.com/42Paris/minilibx-linux.git $(MLX_DIR) 2>/dev/null || true; \
	fi
	@git submodule update --init --recursive $(MLX_DIR)
	@$(MAKE) --silent -C $(MLX_DIR)

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
	@echo "$(GREEN) Successfully compiled minishell.$(RESET)"
	@echo

.c.o:
	@${CC} ${FLAGS} ${IFLAGS}  -c $< -o ${<:.c=.o}
	@clear
	@echo "$(RESET)[$(GREEN)OK$(RESET)]$(BLUE) Compiling $<$(YELLOW)"

clean:
	@${RM} ${OBJS} ${NAME}
	@cd ${LIBFT_DIR}/src && $(MAKE) --silent clean
	@clear
	@echo
	@echo "$(RED)┏┓┓ ┏┓┏┓┳┓┏┓┳┓"
	@echo "┃ ┃ ┣ ┣┫┃┃┣ ┃┃"
	@echo "┗┛┗┛┗┛┛┗┛┗┗┛┻┛"
	@echo

fclean: clean
	@rm -rf ${LIBFT_DIR}
	@rm -f ${NAME}
	@clear
	@echo
	@echo "$(RED)┏┓┓ ┏┓┏┓┳┓┏┓┳┓"
	@echo "┃ ┃ ┣ ┣┫┃┃┣ ┃┃"
	@echo "┗┛┗┛┗┛┛┗┛┗┗┛┻┛"
	@echo

#Leaks check for macos and linux
leak: ${NAME}
ifeq ($(UNAME_S),Linux)
	@echo "$(YELLOW)Running Valgrind for leak check...$(RESET)"
	@$(VALGRIND) ./${NAME} maps/valid/valid1.cub
else ifeq ($(UNAME_S),Darwin)
	@echo "$(YELLOW)Running leak check for macOS...$(RESET)"
	@leaks --atExit -- ./${NAME} maps/valid/valid1.cub
endif

re: fclean all

.PHONY: all bonus clean fclean re test
