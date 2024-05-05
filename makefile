AS = nasm
ASFLAGS = -f elf64

CC = clang
LD = lld
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -fuse-ld=$(LD)
EXECUTABLE = output
SRC = $(wildcard ./src/*.c) $(wildcard ./src/**/*.c) $(wildcard ./src/**/**/*.c)
INCLUDE_DIR = ./inc
BUILD_DIR = ./build

ASM_BUILD_DIR = $(BUILD_DIR)/asm
ASM_BUILD = $(ASM_BUILD_DIR)/output.asm

INC_FILES := $(wildcard ./inc/*.h)

$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(ASM_BUILD_DIR))

all: $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BUILD_DIR)/$(EXECUTABLE)

$(ASM_BUILD): $(BUILD_DIR)/$(EXECUTABLE)
	./$(BUILD_DIR)/$(EXECUTABLE) > $(ASM_BUILD)

debug: $(BUILD_DIR)/$(EXECUTABLE)
	gdb ./$(BUILD_DIR)/$(EXECUTABLE)
	#gdb '/workspaces/C_compiler/build/asm/asm_output'

	
asm: $(ASM_BUILD)
	$(AS) $(ASFLAGS) -o $(ASM_BUILD_DIR)/asm_obj.o $(ASM_BUILD) 
	gcc -g -o $(ASM_BUILD_DIR)/asm_output $(ASM_BUILD_DIR)/asm_obj.o -nostartfiles -no-pie -lc
	./$(ASM_BUILD_DIR)/asm_output

.PHONY: all clean run asm
