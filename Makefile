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
NAME     = cube3d
FLAGS    = -Wall -Wextra -Werror -g -Iincludes
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

# Set MLX flags based on OS
ifeq ($(UNAME_S),Linux)
	MLXFLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lbsd
	MLXINC = -I/usr/include
else ifeq ($(UNAME_S),Darwin)
	MLXFLAGS = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
	MLXINC = 
endif

all: init $(NAME)

# Initialize by updating and compiling submodules
init: init_libft init_mlx

init_libft:
	@echo "$(YELLOW)Initializing Libft...$(RESET)"
	@git submodule update --init --recursive $(LIBFT_DIR)
	@$(MAKE) --silent -C $(LIBFT_DIR)/src

init_mlx:
	@echo "$(YELLOW)Initializing MinilibX...$(RESET)"
	@git submodule update --init --recursive $(MLX_DIR)
	@$(MAKE) --silent -C $(MLX_DIR)

${NAME}: ${OBJS}
	@${CC} ${FLAGS} ${IFLAGS} ${OBJS} ${INCLUDE} ${LIBFT} ${MLX} ${MLXFLAGS} -o ${NAME}
	@echo "$(TITLE)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "$(PURPLE)       ██████╗██╗   ██╗██████╗ ███████╗██████╗ ██████╗         "
	@echo "      ██╔════╝██║   ██║██╔══██╗██╔════╝╚════██╗██╔══██╗        "
	@echo "      ██║     ██║   ██║██████╔╝█████╗   █████╔╝██║  ██║        "
	@echo "      ██║     ██║   ██║██╔══██╗██╔══╝   ╚═══██╗██║  ██║        "
	@echo "      ╚██████╗╚██████╔╝██████╔╝███████╗██████╔╝██████╔╝        "
	@echo "       ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝╚═════╝ ╚═════╝         "
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

test: ${NAME} readline.supp
	${VALGRIND} ./${NAME}

re: fclean all

.PHONY: all bonus clean fclean re test
