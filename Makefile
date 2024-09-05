NAME = ircserv 

FLAG = -Wall -Wextra -Werror -std=c++98

CC = c++

HEADERFILES= inc/Base.hpp inc/Client.hpp inc/Server.hpp inc/TcpSocket.hpp inc/Command.hpp inc/Channel.hpp inc/Replies.hpp
SRC = src/main.cpp \
		src/TcpSocket.cpp \
		src/Server/Server.cpp \
		src/Server/Channel.cpp \
		src/Client/Client.cpp \
		src/Auth/NICK.cpp \
		src/Auth/PASS.cpp \
		src/Auth/USER.cpp \
		src/Commands/Command.cpp \
		src/Commands/join_utils.cpp \
		src/Commands/topic.cpp \
		src/Commands/kick.cpp \
		src/Commands/ClientsUtils.cpp \
		src/Commands/invite.cpp \
		src/Commands/mode_utils.cpp \
		src/Replies/AuthReplies.cpp \
		src/Replies/ChannelsReplies.cpp \
		src/Commands/privmsg.cpp \
		src/Commands/bot.cpp



OBJ = $(SRC:.cpp=.o)

all: $(NAME)

%.o:%.cpp $(HEADERFILES)
	$(CC) $(FLAG) -c $< -o $@

$(NAME):$(OBJ)
	$(CC) -fsanitize=address -g $(FLAG) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all re fclean clean