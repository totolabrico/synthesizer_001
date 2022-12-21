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

SRCS	= 	main.c pcm.c midi.c connect.c \
			oscillator.c oscillatorlst.c note.c notelst.c\
			list_utils.c audiomath_utils.c settings_utils.c

HEADERS	= main.h

CC		= gcc

RM		= rm -f

CFLAGS	= -Wextra -Wall -lasound -lm -pthread -g

${NAME}:	${OBJS}
						gcc -o ${NAME} ${SRCS} ${CFLAGS}

all:		${NAME}

rasp:
		gcc -o ${NAME}32 ${SRCS} -m32 ${CFLAGS}

fclean:
						${RM} $(NAME)

re:						fclean all

.PHONY:		all fclean re
