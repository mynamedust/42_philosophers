NAME	=	philo_bonus
S_DIR	=	src/
O_DIR	=	obj/
SRC		=	main_b.c	init_b.c	utils_b.c	philo_b.c
OBJ		=	$(SRC:.c=.o)
SRCS	=	$(addprefix $(S_DIR), $(SRC))
OBJS	=	$(addprefix $(O_DIR), $(OBJ))
INC		=	-I ./includes
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror

all: $(O_DIR) $(NAME)

$(O_DIR)%.o : $(S_DIR)%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(O_DIR):
	mkdir $(O_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(O_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re