CC = clang
LD = lld
LIBS = -lm -I./DynamicArray/inc
CFLAGS = -Wall -Wextra -std=c99 $(LIBS)
LDFLAGS = -fuse-ld=$(LD)
EXECUTABLE = output
SRC = $(wildcard ./src/*.c) $(wildcard ./src/**/*.c)
INCLUDE_DIR = ./inc
BUILD_DIR = ./build

# Find all header files in inc directory
INC_FILES := $(wildcard ./inc/*.h) $(wildcard ./inc/**/*.h)

# Create build directory if not exists
$(shell mkdir -p $(BUILD_DIR))

all: $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BUILD_DIR)/$(EXECUTABLE)

run: $(BUILD_DIR)/$(EXECUTABLE)
	./$(BUILD_DIR)/$(EXECUTABLE)
