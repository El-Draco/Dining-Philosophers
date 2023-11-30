# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/23 15:21:44 by rriyas            #+#    #+#              #
#    Updated: 2022/09/23 15:50:14 by rriyas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	philo

CC			=	gcc
RM			=	rm -rf
SRCS		=	main.c parse.c initialize.c philo_utils.c eat.c cleanup.c
OBJS		=	${SRCS:.c=.o}


CFLAGS	=	-Wall -Werror -Wextra


$(NAME):	${OBJS}
		${CC} -pthread ${OBJS} -o philo

all:	${NAME}

clean:
		${RM} ${OBJS}

fclean:	clean
		${RM} ${NAME}

re:		fclean all

.PHONY: all clean fclean re



