# Nome do executável
NAME        = minishell

# Compilador e flags
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
RLFLAGS     = -lreadline -lhistory   # Flags para readline

# Diretórios
SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = includes

# Encontra todos os arquivos .c recursivamente em src/
SRCS        = $(shell find $(SRC_DIR) -type f -name '*.c')
# Gera os paths dos .o trocando 'src/' por 'obj/' e .c por .o
OBJS        = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Regra principal
all: $(OBJ_DIR) $(NAME)

# Cria a estrutura de pastas em obj/ (espelhando src/)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	@# Espelha a estrutura de subpastas de src/ em obj/
	$(shell find $(SRC_DIR) -type d -exec mkdir -p $(OBJ_DIR)/{} \; 2>/dev/null)

# Linka os objetos para criar o executável
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(RLFLAGS)

# Compila cada .c em .o, mantendo a estrutura de subpastas
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)  # Cria a subpasta necessária (ex: obj/prompt/)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR)
	
# Limpa
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean