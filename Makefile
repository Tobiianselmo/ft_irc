NAME = ircserv

CC = c++

FLAGS = -Wall -Werror -Wextra -std=c++98 -g

RM = rm -rf

FILES = main.cpp \
Server.cpp \
Client.cpp \
Channel.cpp \
utils.cpp \

SRC_DIR = src/

SRC_FILES = $(addprefix $(SRC_DIR), $(SRC))

OBJ = $(SRC:.cpp=.o)

OBJ_DIR = obj/

OBJ_FILES = $(addprefix $(OBJ_DIR), $(OBJ))

$(NAME): $(OBJ_FILES)
	@$(CC) $(FLAGS) $(OBJ_FILES) -o $(NAME)
	
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(OBJ_DIR)
	@$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

clean:
	@ $(RM) $(OBJ_DIR)

fclean: clean
	@ $(RM) $(NAME)

re : fclean all

.PHONY: all re fclean clean
