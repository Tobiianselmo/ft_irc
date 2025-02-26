NAME = ircserv

CC = c++

FLAGS = -Wall -Werror -Wextra -std=c++98 -g

RM = rm -rf

SRC = main.cpp

SRC_DIR = Src/

SRC_FILES = $(addprefix $(SRC_DIR), $(SRC))

OBJ = $(SRC:.cpp=.o)

OBJ_DIR = Obj/

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