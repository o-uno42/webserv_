# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tjuvan <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/05 15:33:45 by tjuvan            #+#    #+#              #
#    Updated: 2025/05/12 11:34:35 by thiew            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


C = g++
CFLAGS = -std=c++98 -g
FLAGSEXTRA = -Wall -Wextra -Werror
INC_DIR = includes
SRC_DIR = srcs
OBJ_DIR = objs
HEADERS = $(wildcard $(INC_DIR)/*.hpp) 
# SRC = $(wildcard $(SRC_DIR)/*.cpp)
SRC = $(shell find $(SRC_DIR) -name '*.cpp')
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
CFLAGS_ONLY_OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o.cflags_only)
NAME = webserv

all : $(NAME)

$(NAME): $(HEADERS) $(OBJ)
	$(C) $(CFLAGS) $(FLAGSEXTRA) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir	-p $(dir $@)
	$(C) $(CFLAGS) $(FLAGSEXTRA) -c $< -o $@

noextra: $(HEADERS) $(CFLAGS_ONLY_OBJ)
	$(C) $(CFLAGS) $(CFLAGS_ONLY_OBJ) -o $(NAME)
	
install:
	cd public/cgi-bin && npm install

uninstall:
	rm -rf public/cgi-bin/node_modules

$(OBJ_DIR)/%.o.cflags_only : $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(C) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

clean_noextra:
	rm -f $(CFLAGS_ONLY_OBJ)

fclean: clean
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all
