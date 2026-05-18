CC := gcc

TARGET := PROJ_NAME

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

CFLAGS := -O0 -Wall -Wextra -pedantic -g \
          -std=c2x -I$(INC_DIR)

LDFLAGS :=

SRC := $(wildcard $(SRC_DIR)/*.c)

OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

HEADERS := $(wildcard $(INC_DIR)/*.h)

.PHONY: all clean cl run

all: $(TARGET) cl

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

cl:
	rm -rf $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: all
	./$(TARGET)
