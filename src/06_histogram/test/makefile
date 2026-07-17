EXEC_NAME := a.out

CXX := g++

CXXFLAGS += -Wall -Wextra -pedantic
CXXFLAGS += -ggdb3
CXXFLAGS += -fsanitize=thread

LDFLAGS += -fsanitize=thread

SRC_FILES := $(wildcard *.cpp)  $(filter-out ../example.c,$(wildcard ../*.c)) $(wildcard ../*.cpp) 

.PHONY: all clean test

all: $(EXEC_NAME)

$(EXEC_NAME): $(SRC_FILES)
	$(CXX) -x c++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

test: $(EXEC_NAME)
	./$(EXEC_NAME)

clean:
	rm -f $(EXEC_NAME)
