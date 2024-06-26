# program name #
NAME = philo

# shell #
SHELL := /bin/bash

# misc #
COUNT = -1
SLEEP := sleep 0.2

# colors #
DEFAULT=\033[39m
BLACK=\033[30m
DARK_RED=\033[31m
DARK_GREEN=\033[32m
DARK_YELLOW=\033[33m
DARK_BLUE=\033[34m
DARK_MAGENTA=\033[35m
DARK_CYAN=\033[36m
LIGHT_GRAY=\033[37m
DARK_GRAY=\033[90m
RED=\033[91m
GREEN=\033[92m
ORANGE=\033[93m
BLUE=\033[94m
MAGENTA=\033[95m
CYAN=\033[96m
WHITE=\033[97m
BG_DEFAULT=\033[49m
BG_BLACK=\033[40m
BG_DARK_RED=\033[41m
BG_DARK_GREEN=\033[42m
BG_DARK_YELLOW=\033[43m
BG_DARK_BLUE=\033[44m
BG_DARK_MAGENTA=\033[45m
BG_DARK_CYAN=\033[46m
BG_LIGHT_GRAY=\033[47m
BG_DARK_GRAY=\033[100m
BG_RED=\033[101m
BG_GREEN=\033[102m
BG_ORANGE=\033[103m
BG_BLUE=\033[104m
BG_MAGENTA=\033[105m
BG_CYAN=\033[106m
BG_WHITE=\033[107m
RESET=\033[0m
RESET_TERM=\r\033[K

# messages #
MANDATORY = Program compiled
LBONUS = Bonus Program compiled
CLEAN = Objects delete
FCLEAN = Program delete
LIBNAME = philo
BLIBNAME = philo_bonus
COMP = Compiling

# debug and normal flags #
DFLAGS = -Wall -Wextra -Werror -g3 # TO DEBBUG
CFLAGS = -Wall -Werror -Wextra -g3 -pedantic -flto -MD -MP # FOR DEPENDENCIES
LFLAGS = -march=native # TO OPTIMIZE FOR SPECIFIC ARCHITECTURE
FFLAGS = -lreadline # FLAGS THAT ONLY WORK AT THE END OF LINE (AFTER OBJECTS)

# paths #
SRC = philo
BONUS = $(SRC)_bonus/
INC = inc
OBJ = obj

# includes #
HEADERS = $(addprefix $(INC)/, philo.h)

# files path #
EXAMPLE = $(SRC)/EXAMPLE

# libs #
INCLUDES = -I$(INC)/

# main #
MAIN_SRC = ./philo/philo.c

# files mandatory #
CFILES += $(addprefix $(EXAMPLE)/, test.c)

VAL = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt
VAL_TXT = valgrind-out.txt

# obj dir #
OBJECT = $(CFILES:%.c=$(OBJ)/%.o)
BIN_OBJ = $(MAIN_SRC:%.c=$(OBJ)/%.o)

# define bonus #
ifdef WITH_BONUS
	NAME = $(BLIBNAME)
	compile = compile_bonus
	MANDATORY = $(LBONUS)
	MAGENTA = $(YELLOW)
	LIBNAME = $(BLIBNAME)
endif


# define debbug #
ifdef WITH_DEBBUG
	CFLAGS = $(DFLAGS)
endif

# functions #
define create_objects_dir
	mkdir -p $(dir $@)
endef

define compile
	$(CC) -o $(NAME) $(CFLAGS) $(LFLAGS) $(INCLUDES) $(LINCLUDES) $(OBJECT) $(BIN_OBJ) $(LIBFT) $(FFLAGS)
	$(SLEEP)
	printf "\n$(MAGENTA)$(MANDATORY)\n$(RESET)"
endef

define compile_bonus
	$(CC) -o $(NAME) $(CFLAGS) $(LFLAGS) $(INCLUDES) $(LINCLUDES) $(OBJECT) $(BIN_OBJ) $(LIBFT) $(FFLAGS)
	$(SLEEP)
	printf "\n$(MAGENTA)$(MANDATORY)\n$(RESET)"
endef

define compile_source
	$(eval COUNT=$(shell expr $(COUNT) + 1))
	$(MAKE) -sC $(LIBFT_PATH)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<
	printf "$(RESET_TERM)%d%%  $(GREEN)$(COMP) $(notdir $<) -> $(notdir $@) to $(NAME) \r$(RESET)" $$(echo $$(($(COUNT) * 100 / $(words $(OBJECT)))))
endef

define clean
	$(MAKE) fclean -sC $(LIBFT_PATH)
	$(RM) -rf $(OBJ)/ && $(RM) -rf $(VAL_TXT)
	$(SLEEP)
	printf "$(RED)$(CLEAN)$(RESET)\n"
endef

define fclean
	$(call eraseBins)
	@$(SLEEP)
	@printf "$(RED)$(FCLEAN)$(RESET)\n"
endef

define bonus
	@make WITH_BONUS=TRUE -s
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

bonus:
	$(call bonus)

debug:
	$(call debug)

.PHONY: all bonus clean fclean re debug Makefile
.DEFAULT_GOAL := all
.SILENT: