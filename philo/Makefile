# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vkinaret <vkinaret@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/21 14:22:07 by vkinaret          #+#    #+#              #
#    Updated: 2024/03/21 14:22:09 by vkinaret         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CFILES = philo.c utils.c routine.c monitor.c action.c
OFILES = ${CFILES:.c=.o}
FLAGS = -Wall -Wextra -Werror

all: ${NAME}

${NAME}: ${OFILES}
	cc ${OFILES} -o ${NAME}

${OFILES}: ${CFILES}
	cc ${FLAGS} -c ${CFILES} -I philo.h

clean:
	rm -f ${OFILES}

fclean: clean
	rm -f ${NAME}

re: fclean all
