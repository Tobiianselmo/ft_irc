<<<<<<< HEAD
=======
GREEN = \033[0;32m
RED = \033[0;31m
RST = \033[0m

>>>>>>> dev
NAME = ircserv

CC = c++

<<<<<<< HEAD
FLAGS = -Wall -Werror -Wextra -std=c++98 -g

RM = rm -rf

SRC = 	main.cpp \
		Server.cpp \

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
=======
CFLAGS = -Wall -Werror -Wextra -std=c++98

RM = rm -rf

FILES = main.cpp \
Server.cpp \
Client.cpp \

SRC = $(addprefix src/, $(FILES))

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "$(GREEN)  ✔  FT_IRC  is successfully compiled!$(RST)"

clean:
	@$(RM) $(OBJ)
	@echo "$(RED)Object files removed!$(RST)"

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
>>>>>>> dev
