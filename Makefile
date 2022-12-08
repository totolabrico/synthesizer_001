# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbarbe <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/25 16:29:22 by tbarbe            #+#    #+#              #
#    Updated: 2022/07/25 18:21:31 by tbarbe           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= synth

SRCS	= main.c alsa_utils.c oscillator_utils.c list_utils.c

HEADERS	= main.h

CC		= gcc

RM		= rm -f

CFLAGS	= -Wextra -Wall -lasound -lm

${NAME}:	${OBJS}
						gcc -o ${NAME} ${SRCS} ${CFLAGS}

all:		${NAME}

fclean:
						${RM} $(NAME)

re:						fclean all

.PHONY:		all fclean re
