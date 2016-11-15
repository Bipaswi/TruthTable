CC_FLAGS=-std=c99 -Wall -Wextra -g

all: ttable

ttable: ttable.o
	clang $(CC_FLAGS) ttable.o -o ttable

ttable.o: ttable.c
	clang $(CC_FLAGS) ttable.c -c -o ttable.o

