CC = gcc
CFLAGS = -I./include -Wall -Wextra

SRC_DIR = src
OBJ_DIR = obj

SRCS = main.c $(SRC_DIR)/rbtree.c $(SRC_DIR)/dict.c
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/rbtree.o $(OBJ_DIR)/dict.o

TARGET = dictionary_app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/main.o: main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/rbtree.o: $(SRC_DIR)/rbtree.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/rbtree.c -o $(OBJ_DIR)/rbtree.o

$(OBJ_DIR)/dict.o: $(SRC_DIR)/dict.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/dict.c -o $(OBJ_DIR)/dict.o

clean:
	rm -rf $(OBJ_DIR) $(TARGET) dictionary.txt

.PHONY: all clean