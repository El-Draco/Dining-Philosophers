NAME		=	philo

LIB			=	libftprintf.a
CC			=	gcc
RM			=	rm -rf
SRCS		=	main.c parse.c
LIB_SRCS	=	ft_printf.c ft_printf_utils.c
LIB_OBJS	=	${LIB_SRCS:.c=.o}
OBJS		=	${SRCS:.c=.o}


CFLAGS	=	#-Wall -Werror -Wextra


$(NAME):	${OBJS} ${LIB_OBJS} ${LIB}
		${CC} -pthread ${OBJS} ${LIB} -o philo


$(LIB):		${LIB_OBJS}
		ar rcs ${LIB} ${LIB_OBJS}

all:	${NAME}

clean:
		${RM} ${OBJS} ${LIB_OBJS} ${LIB}

fclean:	clean
		${RM} ${NAME}

re:		fclean all

.PHONY: all clean fclean re



