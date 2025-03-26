GREEN = \033[0;32m
RED = \033[0;31m
RST = \033[0m

NAME = ircserv

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98

RM = rm -rf

SRC_DIR = src/

OBJ_DIR = obj/

SRC_FILES = main \
Server \
Client \
Channel \
utils \
response \
send \
cmd/join \
cmd/invite \
cmd/topic \
cmd/kick \
cmd/clientCommands \
cmd/quit \
cmd/mode \
cmd/privmsg \
cmd/info \
cmd/bot \
# cmd/cut \

SRC = $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC_FILES)))

OBJ = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))

OBJF = .cache_exits

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)  ✔  FT_IRC  is successfully compiled!$(RST)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp | $(OBJF)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJF):
		@mkdir -p $(OBJ_DIR)

clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(RED)Object files removed!$(RST)"

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re