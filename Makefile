# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yilin <yilin@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 14:47:59 by yilin             #+#    #+#              #
#    Updated: 2025/01/08 16:23:05 by yilin            ###   ########.fr        #
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

BUILD_DIR = build_to_exec
BUILTIN_DIR = builtins
EXEC_DIR = exec
LEX_DIR = lexing
PRS_DIR = parsing
#TESTER_DIR = testers

BUILD_FL = $(addprefix $(BUILD_DIR)/, build_args.c  build_filename.c  build_helper.c  build_to_exec.c)
BUILTIN_FL = $(addprefix $(BUILTIN_DIR)/, builtins_env.c builtins_error2.c ft_cd.c ft_env_pwd.c ft_export.c builtins_error1.c builtins_handler.c ft_echo.c ft_exit.c ft_unset.c)
EXEC_FL = $(addprefix $(EXEC_DIR)/, exec.c exec_err.c exec_redirs.c exec_toolbox1.c exec_toolbox2.c exec_utils.c exec_execution.c)
LEX_FL = $(addprefix $(LEX_DIR)/, lexing.c  lexing_helper.c)
PRS_FL = $(addprefix $(PRS_DIR)/, expansion.c expansion_helper.c parsing.c quotes.c expansion_env.c heredoc.c parsing_helper.c quotes_helper.c)
MAIN_FL = main.c main_setup.c main_check_free.c env.c signal.c tokens.c 
#TESTER_FL = $(addprefix $(TESTER_DIR)/, tester_functions.c  testers_build.c  testers.c)

# **************************************************************************** #
#                              SRC & OBJ FILES                                 #
# **************************************************************************** #

# CANNOT ADDPREFIX (MAYBE)
SRCS = $(addprefix $(SRCS_DIR)/, $(BUILTIN_FL) $(BUILD_FL) $(EXEC_FL) $(LEX_FL) $(PRS_FL) $(MAIN_FL) ) #######$(TESTER_FL)
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
	$(print_flag)

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
PSTL_YELLOWB = \033[1;38;2;255;253;208m
PSTL_ORGB = \033[1;38;2;255;179;102m
PSTL_PURPLEB =\033[1;38;2;204;153;255m

GREEN_BBG = \033[1;42m
BLUE_BBG = \033[1;44m
YELLOW_BBG = \033[1;43m
ORANGE_BBG = \033[1;48;5;214m

# **************************************************************************** #
#                               PRINT FLAG                                     #
# **************************************************************************** #

define print_flag
	@echo "$(PSTL_PURPLEB).☆.｡.:*・°･.｡*･.｡* .☆.｡.:*.☆.｡.:*・°･.｡*･.｡*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*$(COLOR_RESET)"
	@echo "$(ROSEB)"
	@cat ascii_art.txt
	@echo "$(COLOR_RESET)"
	@echo "$(BABEBLUEB)"
	@cat ascii_txt.txt
	@echo "$(COLOR_RESET)"
	@echo "$(PSTL_PURPLEB).☆.｡.:*・°･.｡*･.｡* .☆.｡.:*.☆.｡.:*・°･.｡*･.｡*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*$(COLOR_RESET)"
endef