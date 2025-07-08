
# Folders
BOOT_FOLDER			=  boot
BUILD_FOLDER		=  build
SRC_FOLDER			=  src
KERNEL_FOLDER		=  $(SRC_FOLDER)/kernel

# Tools
GCC					=  /usr/bin/gcc
LD					=  /usr/bin/ld

# for using __attribute__((interrupt)):
# -mgeneral-regs-only 
# -Werror
GCC_FLAGS			=  -m32 -ffreestanding -fno-pic -fno-pie -fno-exceptions -Werror -mgeneral-regs-only 
GCC_FLAGS_DBG		=  -m32 -ffreestanding -fno-pic -fno-pie -fno-exceptions -Werror -g
LD_FLAGS			=  -melf_i386 -Ttext 0x8000 -e kmain -nostdlib --oformat binary

# debug interrupts
QEMU_FLAG_DBG_INT	=  -d int

# Source files
BOOTLOADER_SRC 		=  $(BOOT_FOLDER)/bootloader.nasm
C_SRC				:= $(shell find $(SRC_FOLDER) -name '*.c')
NASM_SRC			:= $(shell find $(SRC_FOLDER) -name '*.nasm')

# Output files
BOOTLOADER_BIN 	    =  $(BUILD_FOLDER)/bootloader.bin
KERNEL_BIN 	        =  $(BUILD_FOLDER)/kernel.bin
KERNEL_BIN_DBG 	    =  $(BUILD_FOLDER)/kernel.dbg.bin
OS_IMAGE			=  $(BUILD_FOLDER)/nos_x32.bin
OS_IMAGE_DBG		=  $(BUILD_FOLDER)/nos_x32.dbg.bin

C_OBJ				:= $(patsubst $(SRC_FOLDER)/%.c,$(BUILD_FOLDER)/%.o,$(C_SRC))
NASM_OBJ			:= $(patsubst $(SRC_FOLDER)/%.nasm,$(BUILD_FOLDER)/%.o,$(NASM_SRC))

C_OBJ_DBG			:= $(patsubst $(SRC_FOLDER)/%.c,$(BUILD_FOLDER)/%.dbg.o,$(C_SRC))
DBG_SYMBOLS			:= $(patsubst $(BUILD_FOLDER)/%.dbg.o,$(BUILD_FOLDER)/%.sym,$(C_OBJ_DBG))


# hardcoded hell nah
KERNEL_OBJ			:= $(BUILD_FOLDER)/kernel/kernel.o
OTHER_OBJ			:= $(filter-out $(KERNEL_OBJ), $(C_OBJ))

KERNEL_OBJ_DBG		:= $(BUILD_FOLDER)/kernel/kernel.dbg.o
OTHER_OBJ_DBG		:= $(filter-out $(KERNEL_OBJ_DBG), $(C_OBJ_DBG))

.PHONY: all clean run

all: $(OS_IMAGE)

$(BUILD_FOLDER):
	mkdir -p $(BUILD_FOLDER)

$(BOOTLOADER_BIN): $(BOOTLOADER_SRC)
	nasm -f bin -o $@ $<

$(BUILD_FOLDER)/%.o: $(SRC_FOLDER)/%.c
	mkdir -p $(dir $@)
	$(GCC) $(GCC_FLAGS) -c $< -o $@

$(BUILD_FOLDER)/%.o: $(SRC_FOLDER)/%.nasm
	mkdir -p $(dir $@)
	nasm -f elf32 -o $@ $<

$(KERNEL_BIN): $(C_OBJ) $(NASM_OBJ)
	$(LD) $(LD_FLAGS) $(KERNEL_OBJ) ${OTHER_OBJ} $(NASM_OBJ) -o $@ 
	truncate -s 5120 $@

$(OS_IMAGE): $(BUILD_FOLDER) $(BOOTLOADER_BIN) $(KERNEL_BIN)
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $@

$(BUILD_FOLDER)/%.dbg.o: $(SRC_FOLDER)/%.c
	mkdir -p $(dir $@)
	$(GCC) $(GCC_FLAGS_DBG) -c $< -o $@

$(BUILD_FOLDER)/%.sym: $(BUILD_FOLDER)/%.dbg.o
	objcopy --only-keep-debug $< $@
	objcopy --strip-debug $<

$(KERNEL_BIN_DBG): $(C_OBJ_DBG) $(NASM_OBJ) $(DBG_SYMBOLS)
	$(LD) $(LD_FLAGS) $(KERNEL_OBJ_DBG) ${OTHER_OBJ_DBG} $(NASM_OBJ) -o $@ 
	truncate -s 5120 $@

$(OS_IMAGE_DBG): $(BUILD_FOLDER) $(BOOTLOADER_BIN) $(KERNEL_BIN_DBG)
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN_DBG) > $@

clean: 
	rm -rf $(BUILD_FOLDER)

run: $(OS_IMAGE)
	qemu-system-i386 $<
	make clean

dbg: $(OS_IMAGE_DBG)

run-dbg: $(OS_IMAGE_DBG)
	qemu-system-i386 $< -s -S &
	gdb -q -iex "set auto-load safe-path /"
	make clean