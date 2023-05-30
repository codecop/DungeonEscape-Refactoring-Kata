CC := gcc
CFLAGS := -g -Wall -Wextra -pedantic -std=c99 -O -fprofile-arcs -ftest-coverage
LDFLAGS := -fprofile-arcs

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
COVERAGE := $(SRCS:.c=.c.gcov)

.PHONY: all clean coverage

all: DungeonEscape

DungeonEscape: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

coverage: clean all
	./DungeonEscape
	gcov -o . $(SRCS)

clean:
	rm -f $(OBJS) *.gcda *.gcno $(COVERAGE) DungeonEscape.exe
