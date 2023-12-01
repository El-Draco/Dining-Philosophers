# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/23 15:21:44 by rriyas            #+#    #+#              #
#    Updated: 2023/12/01 21:19:57 by rriyas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	philo_bonus

CC			=	gcc
RM			=	rm -rf
SRCS		=	main.c parser.c initialize.c philo_utils.c cleanup.c eat.c
OBJS		=	${SRCS:.c=.o}


CFLAGS	=	-Wall -Werror -Wextra -g3


$(NAME):	${OBJS}
		${CC} -pthread ${OBJS} -o philo

all:	${NAME}

clean:
		${RM} ${OBJS}

fclean:	clean
		${RM} ${NAME}

re:		fclean all

.PHONY: all clean fclean re



