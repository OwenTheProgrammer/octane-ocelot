CC		:= gcc
CFLAGS	:= -Wall -std=gnu17

.PHONY: debug release buildfs clean run

ROOT_BIN_DIR = bin
ROOT_OBJ_DIR = obj

ROOT_LIB_DIR = lib
COMP_LIB_DIR = $(ROOT_LIB_DIR)/$(COMP_TARGET)

ROOT_SRC_DIR = src
ROOT_INC_DIR = include

# == FINAL BINARIES == #
CLI_BINARY	= $(ROOT_BIN_DIR)/ocelot
OCE_BINARY	= $(ROOT_BIN_DIR)/ocelot_editor

COMP_TARGET := debug

debug: COMP_TARGET := debug
debug: CFLAGS += -O0 -ggdb -Wno-unused-function -Wno-unused-variable
debug: buildfs | $(OCE_BINARY) $(CLI_BINARY)

release: COMP_TARGET := release
release: buildfs | $(OCE_BINARY) $(CLI_BINARY)

# == GLAD COMPILATION == #

GLAD_ROOT_DIR	= $(COMP_LIB_DIR)/glad
GLAD_CFLAGS		= $(CFLAGS) -I$(GLAD_INC_DIR)

GLAD_BIN_DIR	= $(ROOT_OBJ_DIR)/glad
GLAD_SRC_DIR	= $(GLAD_ROOT_DIR)/src
GLAD_INC_DIR	= $(GLAD_ROOT_DIR)/include

GLAD_BINARY		= $(GLAD_BIN_DIR)/glad_$(COMP_TARGET).o

# Compile the object file
$(GLAD_BINARY): $(GLAD_SRC_DIR)/glad.c
	$(CC) -c $< -o $@ $(GLAD_CFLAGS)

# == OCELOT BACKEND COMPILATION == #

OCL_CFLAGS	= $(CFLAGS) -Werror -I$(ROOT_INC_DIR)

OCL_SRC_DIR	= $(ROOT_SRC_DIR)/ocelot
OCL_OBJ_DIR	= $(ROOT_OBJ_DIR)/ocelot

OCL_SRC_FILES	= $(wildcard $(OCL_SRC_DIR)/*.c)
OCL_OBJ_FILES	= $(OCL_SRC_FILES:$(OCL_SRC_DIR)/%.c=$(OCL_OBJ_DIR)/%.o)

$(OCL_OBJ_DIR)/main_cli.o: $(ROOT_SRC_DIR)/main_cli.c
	$(CC) -c $< -o $@ $(OCL_CFLAGS)

$(OCL_OBJ_DIR)/%.o: $(OCL_SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(OCL_CFLAGS)

# == OCTANE COMPILATION == #

OCT_CFLAGS	= $(CFLAGS) -Werror -I$(ROOT_INC_DIR)

OCT_SRC_DIR	= $(ROOT_SRC_DIR)/octane
OCT_OBJ_DIR	= $(ROOT_OBJ_DIR)/octane

OCT_SRC_FILES	= $(wildcard $(OCT_SRC_DIR)/*.c)
OCT_OBJ_FILES	= $(OCT_SRC_FILES:$(OCT_SRC_DIR)/%.c=$(OCT_OBJ_DIR)/%.o)

$(OCT_OBJ_DIR)/%.o: $(OCT_SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(OCT_CFLAGS)

# == OCELOT EDITOR COMPILATION == #

OCE_CFLAGS	= $(CFLAGS) -Werror -I$(ROOT_INC_DIR) -I$(GLAD_INC_DIR) -lglfw -lGL

OCE_SRC_DIR	= $(ROOT_SRC_DIR)/engine
OCE_OBJ_DIR	= $(ROOT_OBJ_DIR)/engine

OCE_SRC_FILES	= $(wildcard $(OCE_SRC_DIR)/*.c)
OCE_OBJ_FILES	= $(OCE_SRC_FILES:$(OCE_SRC_DIR)/%.c=$(OCE_OBJ_DIR)/%.o)

$(OCE_OBJ_DIR)/%.o: $(OCE_SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(OCE_CFLAGS)

$(OCE_OBJ_DIR)/main_editor.o: $(ROOT_SRC_DIR)/main_editor.c
	$(CC) -c $< -o $@ $(OCE_CFLAGS)

# == FINAL BINARY TARGETS == #

$(OCE_BINARY): $(GLAD_BINARY) $(OCT_OBJ_FILES) $(OCL_OBJ_FILES) $(OCE_OBJ_FILES) $(OCE_OBJ_DIR)/main_editor.o
	$(CC) $^ -o $@ $(OCE_CFLAGS)

$(CLI_BINARY): $(OCL_OBJ_FILES) $(OCT_OBJ_FILES) $(OCL_OBJ_DIR)/main_cli.o
	$(CC) $^ -o $@ $(CFLAGS)

run: $(debug)
	./$(OCE_BINARY)

buildfs:
	mkdir -p $(ROOT_BIN_DIR) $(ROOT_OBJ_DIR) $(GLAD_BIN_DIR) $(OCL_OBJ_DIR) $(OCT_OBJ_DIR) $(OCE_OBJ_DIR)

clean:
	rm -rf $(ROOT_OBJ_DIR)
	rm -f $(ROOT_BIN_DIR)/ocelot_*
