GREEN = \033[0;32m
RED = \033[0;31m
RST = \033[0m

NAME = ircserv

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98

RM = rm -rf

FILES = main.cpp \
Server.cpp \
Client.cpp \
Channel.cpp \
utils.cpp \
join.cpp \
invite.cpp \
topic.cpp \
response.cpp \
send.cpp \
kick.cpp \
clientCommands.cpp \
quit.cpp \
# mode.cpp \
cut.cpp \

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