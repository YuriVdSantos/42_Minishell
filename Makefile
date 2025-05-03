NAME = minishell

SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = Libft42

SRC = $(wildcard $(SRC_DIR)/*.c) \
      $(wildcard $(SRC_DIR)/parser/*.c) \
      $(wildcard $(SRC_DIR)/builtins/*.c) \
      $(wildcard $(SRC_DIR)/signals/*.c) \
      $(wildcard $(SRC_DIR)/executor/*.c) \
      $(wildcard $(SRC_DIR)/utils/*.c)

OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iincludes -I$(LIBFT_DIR)/includes
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline -lncurses

# CFLAGS = -Wall -Wextra -Werror -Iincludes -I$(LIBFT_DIR)/includes -I/opt/homebrew/opt/readline/include
# LDFLAGS = -L$(LIBFT_DIR) -lft -L/opt/homebrew/opt/readline/lib -lreadline -lncurses

all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(LIBFT_DIR)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Minishell compiled successfully!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re