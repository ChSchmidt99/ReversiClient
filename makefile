# type: "make" to build the main file and all dependencies. Execute with build/play
# tpye: "make test" to build the main test file and all dependencies. Execute with build/test
# tpye: "make clean" to delete all previously compiled binaries

CC=gcc

TARGET_EXEC ?= play
TEST_EXEC ?= test

TEST_MAIN ?= test/test_main.c
PROG_MAIN ?= src/main.c
TEST_MAINO ?= test_main.o
PROG_MAINO ?= main.o

BUILD_DIR ?= build
SRC_DIRS ?= src

SRCS := $(shell find $(SRC_DIRS) -name *.c ! -name main.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_FLAGS := -Iinclude
LDFLAGS ?= $(INC_FLAGS) -Wall -Wextra -Werror

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(BUILD_DIR)/$(PROG_MAINO)
	$(CC) $(OBJS) $(BUILD_DIR)/$(PROG_MAINO) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) -c $< -o $@ $(LDFLAGS)

$(TEST_EXEC): $(OBJS) $(BUILD_DIR)/$(TEST_MAINO)
	$(CC) $(OBJS) $(BUILD_DIR)/$(TEST_MAINO) -o $(BUILD_DIR)/$@ $(LDFLAGS)

.PHONY: $(BUILD_DIR)/$(TEST_MAINO)

$(BUILD_DIR)/$(TEST_MAINO): $(TEST_MAIN)
	$(CC) -c $< -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(PROG_MAINO): $(PROG_MAIN)
	$(CC) -c $< -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR) 

MKDIR_P ?= mkdir -p