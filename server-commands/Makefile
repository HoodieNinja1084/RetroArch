CC = gcc
CFLAGS = -W -Wall -ggdb3 -I./headers
NAME = server
SRC = main.c \
      xfunc.c \
      init.c \
      netCommandsHandler.c \
      netPacketsDef.c
OBJS = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all:	$(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re:	fclean all 
