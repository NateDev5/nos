# Folders
BOOT_FOLDER			= ./boot
BUILD_FOLDER		= ./build
KERNEL_FOLDER		= ./kernel

# Source files
BOOTLOADER_X32_SRC 	= $(BOOT_FOLDER)/bootloader.nasm
KERNEL_X32_SRC 	    = $(KERNEL_FOLDER)/kernel.c

# Output files
BOOTLOADER_X32_BIN 	= $(BUILD_FOLDER)/bootloader.bin
KERNEL_X32_OBJ 	    = $(BUILD_FOLDER)/kernel.o
KERNEL_X32_BIN 	    = $(BUILD_FOLDER)/kernel.bin
OS_IMAGE			= $(BUILD_FOLDER)/nos_x32.bin

# Other
#GCC				= ${HOME}/cross/bin/$(target)-gcc 
#LD					= ${HOME}/cross/bin/$(target)-ld 
GCC					= /usr/bin/gcc
LD					= /usr/bin/ld

.PHONY: all clean

all: $(OS_IMAGE)

$(BUILD_FOLDER):
	mkdir -p $(BUILD_FOLDER)

$(BOOTLOADER_X32_BIN): $(BOOTLOADER_X32_SRC)
	nasm -f bin -o $@ $<

$(KERNEL_X32_OBJ): $(KERNEL_X32_SRC)
	$(GCC) -m32 -ffreestanding -fno-pic -fno-pie -fno-exceptions -fno-rtti -c $< -o $@ 

$(KERNEL_X32_BIN): $(KERNEL_X32_OBJ)
	$(LD) -melf_i386 -Ttext 0x8000 -nostdlib --oformat binary $< -o $@ 
	truncate -s 5120 $@

$(OS_IMAGE): $(BUILD_FOLDER) $(BOOTLOADER_X32_BIN) $(KERNEL_X32_BIN)
	cat $(BOOTLOADER_X32_BIN) $(KERNEL_X32_BIN) > $@

clean: 
	rm -f $(BOOTLOADER_X32_BIN) $(KERNEL_X32_BIN) $(KERNEL_X32_OBJ) $(OS_IMAGE)

run: $(OS_IMAGE)
	qemu-system-i386 $<