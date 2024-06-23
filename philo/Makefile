# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vkinaret <vkinaret@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/21 14:22:07 by vkinaret          #+#    #+#              #
#    Updated: 2024/06/23 18:30:46 by vkinaret         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CFILES = philo.c utils.c routine.c monitor.c
OFILES = ${CFILES:.c=.o}
FLAGS = -Wall -Wextra -Werror

all: ${NAME}

${NAME}: ${OFILES}
	cc ${OFILES} -o ${NAME}

${OFILES}: ${CFILES}
	cc ${FLAGS} -c ${CFILES}

clean:
	rm -f ${OFILES}

fclean: clean
	rm -f ${NAME}

re: fclean all
