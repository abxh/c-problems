EXEC_NAME := a.out

CC      := gcc
CXX     := g++

CFLAGS  += -Wall -Wextra -pedantic
CFLAGS  += -ggdb3
CFLAGS  += -fsanitize=address

CXXFLAGS += $(CFLAGS)

LDFLAGS += -fsanitize=address

C_FILES   := $(filter-out ../example.c,$(wildcard ../*.c))
CPP_FILES := $(wildcard *.cpp) $(wildcard ../*.cpp)

OBJ_FILES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

.PHONY: all clean test

all: $(EXEC_NAME)

$(EXEC_NAME): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(EXEC_NAME)
	./$(EXEC_NAME)

clean:
	rm -f $(OBJ_FILES) $(EXEC_NAME)
