# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/23 10:00:00 by hmunoz-g          #+#    #+#              #
#    Updated: 2025/08/23 10:00:00 by hmunoz-g         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -=-=-=-=-    COLOURS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

DEF_COLOR   = \033[0;39m
YELLOW      = \033[0;93m
CYAN        = \033[0;96m
GREEN       = \033[0;92m
BLUE        = \033[0;94m
RED         = \033[0;91m

# -=-=-=-=-    NAME -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

NAME        := libftpp.a

# -=-=-=-=-    FLAG -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

CPP          = c++
FLAGS       = -Werror -Wall -Wextra -std=c++17 -g -fsanitize=address
DEPFLAGS    = -MMD -MP

# -=-=-=-=-    PATH -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

RM          = rm -fr
OBJ_DIR     = .obj
DEP_DIR     = .dep

# -=-=-=-=-    FILES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

SRC         := IOStream/thread_safe_iostream.cpp \
			   threading/thread.cpp \
			   threading/worker_pool.cpp \
			   threading/persistent_worker.cpp \
			   network/message.cpp \
			   network/client.cpp \
			   network/server.cpp

OBJS        = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
DEPS        = $(addprefix $(DEP_DIR)/, $(SRC:.cpp=.d))

# -=-=-=-=-    TARGETS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

all: directories $(NAME)

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DEP_DIR)

-include $(DEPS)

$(OBJ_DIR)/%.o: %.cpp 
	@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$*.d)
	$(CPP) $(FLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(DEP_DIR)/$*.d

$(NAME): $(OBJS) Makefile
	@echo "$(GREEN)Linking $(NAME)!$(DEF_COLOR)"
	ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN)$(NAME) compiled!$(DEF_COLOR)"
	@echo "$(RED)I'm a classy programmer$(DEF_COLOR)"

test_unit_1:
	$(CPP) $(FLAGS) basic_tests.cpp $(NAME) -o libftpp_test_1

test_unit_2:
	$(CPP) $(FLAGS) ./IOStream/thread_safe_iostream_tests.cpp $(NAME) -o ./libftpp_test_2

test_unit_3:
	$(CPP) $(FLAGS) ./threading/threading.cpp $(NAME) -o ./libftpp_test_3

test_unit_4:
	$(CPP) $(FLAGS) ./network/network_tests.cpp $(NAME) -o ./libftpp_test_4

clean:
	@$(RM) $(OBJ_DIR) $(DEP_DIR)
	@echo "$(RED)Cleaned object files and dependencies$(DEF_COLOR)"

fclean: clean
	@$(RM) $(NAME)
	@$(RM) libftpp_test_1
	@$(RM) libftpp_test_2
	@$(RM) libftpp_test_3
	@$(RM) libftpp_test_4
	@echo "$(RED)Cleaned all binaries$(DEF_COLOR)"

re: fclean all

.PHONY: all clean fclean re directories
