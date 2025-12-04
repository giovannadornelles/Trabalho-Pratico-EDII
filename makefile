CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC= src/main.c src/huffman.c src/min_heap.c src/search.c src/kmp.c
OBJ= $(SRC: .c = .o)
TARGET= programa

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o ${TARGET}

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)

