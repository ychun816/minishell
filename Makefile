# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 14:47:59 by yilin             #+#    #+#              #
#    Updated: 2024/11/01 14:30:57 by varodrig         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                 NAME / MAIN                                  #
# **************************************************************************** #

NAME = minishell

# **************************************************************************** #
#                                 DIRECTORIES                                  #
# **************************************************************************** #

INCL_DIR = includes
SRCS_DIR = srcs
OBJS_DIR = objs
LIBFT_DIR = libft

# **************************************************************************** #
#                              SRC & OBJ FILES                                 #
# **************************************************************************** #

SRCS = $(addprefix $(SRCS_DIR)/, main.c) #######
#split_quote.c
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

# **************************************************************************** #
#                                 LIBRARY FILES                                #
# **************************************************************************** #

LIBFT_A = $(LIBFT_DIR)/libft.a

# **************************************************************************** #
#                            COMPILER & FLAGS                                  #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -I$(INCL_DIR) -I$(LIBFT_DIR)/includes
LFLAGS = -L$(LIBFT_DIR) -lft
LDFLAGS = -lreadline
RM = rm -rf

# **************************************************************************** #
#                               BUILD COMMANDS                                 #
# **************************************************************************** #

### Build Targets ###
all: $(NAME)

### NAME Create the executable ###
$(NAME): $(OBJS) $(LIBFT_A)
	@$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(NAME) $(LDFLAGS)
	@echo "$(ROSEB)🚀🦄 $(NAME) SUCCESSFULLY COMPILED! 🦄🚀$(COLOR_RESET)"

### Compile object files ###
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREENB)---🔧 COMPILING $< TO $@ 🔧---$(COLOR_RESET)"

### Create object directory ###
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

### Build the libft library ###
$(LIBFT_A):
	@make -C $(LIBFT_DIR)
	@echo "$(ORANGE_BBG) $(LIBFT_DIR) CREATED$(COLOR_RESET)"

### Archive Libraries ###
archives: libft
	@mkdir -p archives
	@if [ -f $(LIBFT_A) ]; then cp $(LIBFT_A) archives/; fi

#--silent: add at the back if don't want it displayed!

### Clean up object files and directories ###
clean:
	@$(RM) $(OBJS_DIR)
	@make clean -C $(LIBFT_DIR)
	@echo "$(BABEBLUEB)🧹 CLEAN DONE! OBJS of [$(NAME)] REMOVED 🧹$(COLOR_RESET)"

### Remove all built files ###
fclean: clean
	@$(RM) $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@echo "$(BABEBLUEB)🫧 FCLEAN DONE! [$(LIBFT_A)] REMOVED 🫧$(COLOR_RESET)"

### Rebuild the project ###
re: fclean all
	@echo "$(REDB)RE DONE$(COLOR_RESET)"

# **************************************************************************** #
#                              PHONY TARGETS                                   #
# **************************************************************************** #

.PHONY: all clean fclean re libft archives

# **************************************************************************** #
#                              COLOR SETTING                                   #
# **************************************************************************** #

COLOR_RESET = \033[0m
PINKB = \033[1;95m
REDB = \033[1;91m
ROSEB = \033[1;38;5;225m
BLUEB = \033[1;34m
BABEBLUEB = \033[1;38;5;153m
GREENB = \033[1;38;5;85m
PURPLEB = \033[1;38;5;55m
GREEN_BBG = \033[1;42m
BLUE_BBG = \033[1;44m
YELLOW_BBG = \033[1;43m
ORANGE_BBG = \033[1;48;5;214m