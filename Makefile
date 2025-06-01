CC		:= gcc
CFLAGS	= -Wall -Werror -std=gnu17 -I$(INC_DIR)
CLIBS	= -lglfw

ifeq ($(PLATFORM), win64)
	CC := x86_64-w64-mingw32-$(CC)
	CFLAGS += -mconsole
	BINARY = $(BIN_DIR)/ocelot_w64
else
	BINARY = $(BIN_DIR)/ocelot_deb
endif

BIN_DIR	= bin
OBJ_DIR	= obj

SRC_DIR	= src
INC_DIR	= include

SRC_FILES = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/ocelot/*.c $(SRC_DIR)/octane/*.c $(SRC_DIR)/gui/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Finds all subdirectories that need to be made for the obj directory
# $(call get_obj_subdirs,<SOURCE DIR>,<OBJECT DIR>)
get_obj_subdirs = $(patsubst $(1)/%,$(2)/%,$(shell find $(1) -mindepth 1 -type d))

.PHONY: clean buildfs debug release run


debug: CFLAGS += -O0 -ggdb -Wno-unused-function -Wno-unused-variable
debug: buildfs | $(BINARY)

release: CFLAGS += -O3 -s -Wno-unused-result
release: buildfs | $(BINARY)

run: $(BINARY)
	./$(BINARY)

$(BINARY): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(CFLAGS) $(CLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CLIBS)


buildfs:
	mkdir -p $(BIN_DIR) $(OBJ_DIR) $(call get_obj_subdirs,$(SRC_DIR),$(OBJ_DIR))

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(BIN_DIR)/ocelot_*
