AS = nasm
ASFLAGS = -f elf64

CC = clang
LD = lld
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -fuse-ld=$(LD)
EXECUTABLE = output
SRC = $(wildcard ./src/*.c) $(wildcard ./src/**/*.c)
INCLUDE_DIR = ./inc
BUILD_DIR = ./build

ASM_BUILD = $(BUILD_DIR)/asm/output.asm

INC_FILES := $(wildcard ./inc/*.h)

$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(BUILD_DIR)/asm)

all: $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): $(SRC)
    $(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ $^ $(LDFLAGS)

clean:
    rm -f $(BUILD_DIR)/$(EXECUTABLE)

asm_output.asm: $(BUILD_DIR)/$(EXECUTABLE)
    ./$(BUILD_DIR)/$(EXECUTABLE) > $(ASM_BUILD)

run: asm_output.asm
    $(AS) $(ASFLAGS) -o $(BUILD_DIR)/executable $(ASM_BUILD)
    ./$(BUILD_DIR)/executable