# Executables
NASM					= nasm
LD						= ld
OBJCOPY					= objcopy

# Flags
LD_FLAGS				= -melf_i386 -Ttext 0x8000 -nostdlib --oformat elf32-i386

# Folders
INCLUDE_FOLDER			= ./
BUILD_FOLDER			= ../bin/bootloader
STAGE1_FOLDER 			= stage1
STAGE2_FOLDER 			= stage2

# Source files
STAGE1_SRC 				= $(STAGE1_FOLDER)/stage1.nasm
STAGE2_SRC 				= $(STAGE2_FOLDER)/stage2.nasm

# Output files
STAGE1_BIN				= $(BUILD_FOLDER)/stage1.bin
STAGE2_OBJ				= $(BUILD_FOLDER)/stage2.o
STAGE2_ELF				= $(BUILD_FOLDER)/stage2.elf
STAGE2_BIN				= $(BUILD_FOLDER)/stage2.bin
BOOTLOADER_BIN 			= $(BUILD_FOLDER)/bootloader.bin

# Other
STAGE1_SIZE				= 512
STAGE2_SIZE				= 5120

.PHONY: all

all: $(BUILD_FOLDER) $(BOOTLOADER_BIN)

$(BUILD_FOLDER):
	mkdir -p $(BUILD_FOLDER)

$(STAGE1_BIN): $(STAGE1_SRC)
	$(NASM) -i$(INCLUDE_FOLDER) -f bin -o $@ $<

$(STAGE2_OBJ): $(STAGE2_SRC)
	$(NASM) -i$(INCLUDE_FOLDER) -f elf32 -o $@ $<

$(STAGE2_ELF): $(STAGE2_OBJ)
	$(LD) $(LD_FLAGS) $< -o $@

$(STAGE2_BIN): $(STAGE2_ELF)
	$(OBJCOPY) -O binary $< $@
	truncate -s $(STAGE2_SIZE) $@

$(BOOTLOADER_BIN): $(STAGE1_BIN) $(STAGE2_BIN)
	cat  $(STAGE1_BIN) $(STAGE2_BIN) > $(BOOTLOADER_BIN)
