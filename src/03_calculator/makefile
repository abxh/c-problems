EXEC_NAME := a.out

CC         := gcc
CFLAGS     += -I./../../lib
CFLAGS     += -Wall -Wextra -pedantic
CFLAGS     += -ggdb3
CFLAGS     += -fsanitize=undefined
CFLAGS     += -fsanitize=address
CFLAGS     += -I./../../data-structures-c/lib

C_FILES     := $(wildcard *.c)
OBJ_FILES   := $(C_FILES:.c=.o)

LD_FLAGS   += -fsanitize=undefined
LD_FLAGS   += -fsanitize=address
LD_FLAGS   += -lm

.PHONY: all clean test

all: $(EXEC_NAME)

clean:
	rm -rf $(OBJ_FILES)
	rm -rf $(EXEC_NAME)

test: $(EXEC_NAME)
	./a.out

$(EXEC_NAME): $(OBJ_FILES)
	$(CC) $(LD_FLAGS) $^ -o $(EXEC_NAME)

$(OBJ_FILES): $(SRC_FILES)

$(SRC_FILES):
	$(CC) -c $(CFLAGS) $@
