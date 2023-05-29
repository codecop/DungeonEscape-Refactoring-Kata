CC := gcc
CFLAGS := -g -Wall -Wextra -pedantic -std=c99 -O

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all clean

all: DungeonEscape

DungeonEscape: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) DungeonEscape.exe
