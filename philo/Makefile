# program name #
NAME = philo

# shell #
SHELL := /bin/bash

# misc #
COUNT := 0
SLEEP := sleep 0.2

# colors #
DEFAULT=\033[39m
RESET_TERM=\r\033[K

# messages #
MANDATORY = $(NAME) compiled
LBONUS = $(NAME)_bonus Program compiled
CLEAN = Objects delete
FCLEAN = Program delete
COMP = Compiling

# debug and normal flags #
DFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=thread# TO DEBBUG
CFLAGS = -Wall -Werror -Wextra -g3 -pedantic -flto -MD -MP # FOR DEPENDENCIES
LFLAGS = -march=native # TO OPTIMIZE FOR SPECIFIC ARCHITECTURE
FFLAGS =  -lpthread -pthread # FLAGS THAT
CCPFLAGS := -I./inc/ -DDEBUG

# paths #
SRC = src
INC = inc
OBJ = obj

# includes #
HEADERS = $(addprefix $(INC)/, philo.h)

# files mandatory #
CFILES = philo.c
CFILES += $(addprefix $(SRC)/, arg_validation.c deallocating_structures.c error_messages.c instantiation_of_structures.c \
			libft_functions.c start_simulation.c utils.c dinner_routine_utils.c monitor_routine.c dinner_routine.c)

VAL = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt
VAL_TXT = valgrind-out.txt

# obj dir #
OBJECT = $(CFILES:%.c=$(OBJ)/%.o)

# define debbug #
ifdef WITH_DEBBUG
	CFLAGS = $(DFLAGS)
endif

# functions #
define create_objects_dir
	mkdir -p $(dir $@)
endef

define compile
	$(CC) -o $(NAME) $(CFLAGS) $(LFLAGS) $(CCPFLAGS) $(OBJECT) $(BIN_OBJ) $(FFLAGS)
	$(SLEEP)
	printf "\n$(MANDATORY)\n"
endef

define compile_bonus
	$(CC) -o $(NAME) $(CFLAGS) $(LFLAGS) $(INCLUDES) $(OBJECT) $(BIN_OBJ) $(FFLAGS)
	$(SLEEP)
	printf "\n$(MANDATORY)\n"
endef

define compile_source
	$(eval COUNT=$(shell expr $(COUNT) + 1))
	$(CC) -o $@ $(CCPFLAGS) $(CFLAGS) $(INCLUDES) -c $<
	printf "$(RESET_TERM)%d%%  $(COMP) $(notdir $<) -> $(notdir $@) to $(NAME) \r" $$(echo $$(($(COUNT) * 100 / $(words $(OBJECT)))))
endef

define clean
	$(RM) -rf $(OBJ)/ && $(RM) -rf $(VAL_TXT)
	$(SLEEP)
	printf "$(CLEAN)\n"
endef

define fclean
	$(call eraseBins)
	@$(SLEEP)
	@printf "$(FCLEAN)\n"
endef

define debug
	$(call clean)
	$(call fclean)
	$(MAKE) WITH_DEBBUG=TRUE -s
endef

define eraseBins
	$(if $(NAME),@$(RM) $(NAME))
	$(if $(BLIBNAME),@$(RM) $(BLIBNAME))
endef

# rules #
all: $(NAME)

$(NAME): $(OBJECT) $(BIN_OBJ)
	$(call create_objects_dir)
	$(call compile)

$(OBJ)/%.o: %.c
	$(call create_objects_dir)
	$(call compile_source)

-include $(OBJECT:.o=.d)

clean:
	$(call clean)

fclean: clean
	$(call fclean)

re: fclean all

debug:
	$(call debug)

.PHONY: all clean fclean re debug Makefile
.DEFAULT_GOAL := all
.SILENT: