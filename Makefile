.PHONY: first-target install uninstall clean

DIR_GUARD=@mkdir -p $(@D)
INC_DIR=src
SRC_DIR=src
OBJ_DIR=obj
ifdef out
PREFIX=$(out)/
endif
BIN_DIR=$(PREFIX)bin
BIN_NAME=oshi
BIN_PATH=$(addprefix $(BIN_DIR)/, $(BIN_NAME))

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(patsubst %.c, %.o, $(SRC)))

INSTALL=cp
CC=gcc
CFLAGS=-I./$(INC_DIR) -lm -std=c99 -Wall -ggdb

first-target: $(BIN_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(DIR_GUARD)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN_NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_PATH): $(BIN_NAME)
ifndef out
	$(warning $$out environment variable not defined, install in current\
	directory instead)
endif
	$(DIR_GUARD)
	$(INSTALL) $(BIN_NAME) $@

install: $(BIN_PATH)

uninstall: $(PREFIX)
ifndef out
	$(error $$out environment variable not defined, no prefix to uninstall)
else
	rm -r $<
endif

clean:
	rm $(BIN_NAME)
	rm -r $(OBJ_DIR)
