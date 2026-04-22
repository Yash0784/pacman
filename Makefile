CC     = gcc
CFLAGS = -Wall -g
LIBS   = -lncursesw
TARGET = pacman_game
SRCS   = mainmenu.c \
         pacman.c   \
         pacman2.c  \
         movement.c \
         ghosts.c   \
         level2_ghosts.c \
         level2_graph.c
OBJS   = $(SRCS:.c=.o)
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
run: all
	./$(TARGET)
clean:
	rm -f $(OBJS) $(TARGET)