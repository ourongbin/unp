# <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
# My Makefile. Auther: ourongbin@gmail.com
#
# 	workroot
# 	├── bin
# 	├── inc
# 	├── obj
# 	└── src
#
# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

BINS=bin/a.out

# <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
# Do not change
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

CC=gcc
CFLAGS=-Wall -I inc/

all: $(BINS)

bin/a.out: $(OBJS)
	$(CC) $(CFLAGS)    $^ -o $@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: all clean _d
clean:
	rm $(OBJS) $(BINS)
_d:
	@echo $(OBJS)
	@echo $(BINS)
# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
