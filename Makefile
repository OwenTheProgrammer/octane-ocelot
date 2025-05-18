CC		= gcc
CFLAGS	= -Wall -Werror -std=gnu17 -I$(INC_DIR)
CLIBS	= -lm

SRC_DIR	= src
OBJ_DIR	= obj

INC_DIR	= include
BIN_DIR	= bin

BINARY	= $(BIN_DIR)/ocelot

SRC_FILES = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/engine/*.c $(SRC_DIR)/filesys/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Finds all subdirectories that need to be created for the
# object directory tree when compiling src
get_obj_subdirs = $(patsubst $(1)/%,$(2)/%,$(shell find $(1) -mindepth 1 -type d))


.PHONY: debug release clean buildfs run

debug: CFLAGS += -O0 -ggdb -Wno-unused-function -Wno-unused-variable -fsanitize=address
debug: buildfs | $(BINARY)

release: CFLAGS += -O3 -s
release: buildfs | $(BINARY)

$(BINARY): $(OBJ_FILES)
	$(CC) $^ -o $@ $(CFLAGS) $(CLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CLIBS)

run: $(BINARY)
	./bin/ocelot

buildfs:
	mkdir -p $(BIN_DIR) $(OBJ_DIR) $(call get_obj_subdirs,$(SRC_DIR),$(OBJ_DIR))

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(BINARY)
