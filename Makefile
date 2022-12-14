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

SRCS	= main.c pcm_utils.c oscillator_utils.c list_utils.c note_utils.c \
				oscillatorlst_utils.c midi_utils.c audiomath_utils.c connect_utils.c

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
