.PHONY: first install clean

DIR_GUARD=mkdir -p $(@D)
INC_DIR=src
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
BIN_NAME=ex_bonus1
BIN_PATH=$(addprefix $(BIN_DIR)/, $(BIN_NAME))

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(patsubst %.c, %.o, $(SRC)))

CC=gcc
CFLAGS=-I./$(INC_DIR) -lm -std=c99 -Wall -ggdb

first: $(BIN_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(DIR_GUARD)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN_NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_PATH): $(BIN_NAME)
	$(DIR_GUARD)
	mv $(BIN_NAME) $(BIN_DIR)

install: $(BIN_PATH)

clean:
	rm $(OBJ_DIR)/*.o
