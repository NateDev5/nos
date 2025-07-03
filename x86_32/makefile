
# Folders
BOOT_FOLDER			=  boot
BUILD_FOLDER		=  build
SRC_FOLDER			=  src
KERNEL_FOLDER		=  $(SRC_FOLDER)/kernel

# Tools
GCC					=  /usr/bin/gcc
LD					=  /usr/bin/ld

GCC_FLAGS			=  -m32 -ffreestanding -fno-pic -fno-pie -fno-exceptions -Werror
LD_FLAGS			=  -melf_i386 -Ttext 0x8000 -e kmain -nostdlib --oformat binary

# Source files
BOOTLOADER_SRC 		=  $(BOOT_FOLDER)/bootloader.nasm
C_SRC				:= $(shell find $(SRC_FOLDER) -name '*.c')

# Output files
BOOTLOADER_BIN 	    =  $(BUILD_FOLDER)/bootloader.bin
KERNEL_BIN 	        =  $(BUILD_FOLDER)/kernel.bin
OS_IMAGE			=  $(BUILD_FOLDER)/nos_x32.bin
C_OBJ				:= $(patsubst $(SRC_FOLDER)/%.c,$(BUILD_FOLDER)/%.o,$(C_SRC))
# hardcoded hell nah
KERNEL_OBJ			:= $(BUILD_FOLDER)/kernel/kernel.o
OTHER_OBJ			:= $(filter-out $(KERNEL_OBJ), $(C_OBJ))

.PHONY: all clean run

all: $(OS_IMAGE)

$(BUILD_FOLDER):
	mkdir -p $(BUILD_FOLDER)

$(BOOTLOADER_BIN): $(BOOTLOADER_SRC)
	nasm -f bin -o $@ $<

$(BUILD_FOLDER)/%.o: $(SRC_FOLDER)/%.c
	mkdir -p $(dir $@)
	$(GCC) $(GCC_FLAGS) -c $< -o $@

$(KERNEL_BIN): $(C_OBJ)
	$(LD) $(LD_FLAGS) $(KERNEL_OBJ) ${OTHER_OBJ} -o $@ 
	truncate -s 5120 $@

$(OS_IMAGE): $(BUILD_FOLDER) $(BOOTLOADER_BIN) $(KERNEL_BIN)
	echo $(C_SRC)
	echo $(C_OBJ)
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $@

clean: 
	rm -f $(BOOTLOADER_BIN) $(KERNEL_BIN) $(OS_IMAGE) $(C_OBJ)

run: $(OS_IMAGE)
	qemu-system-i386 $<
	make clean