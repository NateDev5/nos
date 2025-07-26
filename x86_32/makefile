DEBUG				?= 0

# Folders
BOOT_FOLDER			=  boot
BUILD_FOLDER		=  bin
SRC_FOLDER			=  src
KERNEL_FOLDER		=  $(SRC_FOLDER)/kernel

# Tools
GCC					=  g++#/usr/bin/gcc
LD					=  ld#/usr/bin/ld

# for using __attribute__((interrupt)):
# -mgeneral-regs-only 
# -Werror
# -fno-stack-protector (stack overflows wont be found and it might find bugs if enabled)
GCC_FLAGS			=  -m32 -ffreestanding -fno-pic -fno-pie -fno-exceptions -fno-rtti -fno-stack-protector -Werror -Wall -mgeneral-regs-only
GCC_FLAGS_DBG		=  -g -D__DEBUG__
GCC_FLAGS_GDB		=  -D__GDB__
LD_FLAGS			=  -melf_i386 -Ttext 0x8000 -e kmain -nostdlib --oformat elf32-i386

# debug interrupts
QEMU_FLAG_DBG_INT	=  -d int
QEMU_FLAG_SERIAL	=  -serial stdio

# Source files
BOOTLOADER_SRC 		=  $(BOOT_FOLDER)/bootloader.nasm
CPP_SRC				:= $(shell find $(SRC_FOLDER) -name '*.cpp')
NASM_SRC		    := $(shell find $(SRC_FOLDER) -name '*.nasm')
CPP_HEADERS			:= $(shell find $(SRC_FOLDER) -name '*.h')

# Output files
BOOTLOADER_BIN 	    =  $(BUILD_FOLDER)/bootloader.bin
KERNEL_BIN 	        =  $(BUILD_FOLDER)/kernel.bin
KERNEL_BIN_DBG 	    =  $(BUILD_FOLDER)/kernel.dbg.bin
OS_IMAGE			=  $(BUILD_FOLDER)/nos_x32.bin
OS_IMAGE_DBG		=  $(BUILD_FOLDER)/nos_x32.dbg.bin

CPP_OBJ				:= $(patsubst $(SRC_FOLDER)/%.cpp,$(BUILD_FOLDER)/%.o,$(CPP_SRC))
NASM_OBJ			:= $(patsubst $(SRC_FOLDER)/%.nasm,$(BUILD_FOLDER)/%.o,$(NASM_SRC))

DBG_SYMBOLS			:= $(patsubst $(BUILD_FOLDER)/%.o,$(BUILD_FOLDER)/%.sym,$(CPP_OBJ))

# hardcoded hell nah
KERNEL_OBJ			:= $(BUILD_FOLDER)/kernel/kernel.o
OTHER_OBJ			:= $(filter-out $(KERNEL_OBJ), $(CPP_OBJ))

ifeq ($(DEBUG), 1)
	GCC_FLAGS += $(GCC_FLAGS_DBG)
endif

.PHONY: all clean run

all: $(OS_IMAGE)

$(BUILD_FOLDER):
	mkdir -p $(BUILD_FOLDER)

$(BOOTLOADER_BIN): $(BOOTLOADER_SRC)
	nasm -f bin -o $@ $<

$(BUILD_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp
	mkdir -p $(dir $@)
	$(GCC) $(GCC_FLAGS) -I$(SRC_FOLDER) -c $< -o $@

$(BUILD_FOLDER)/%.o: $(SRC_FOLDER)/%.nasm
	mkdir -p $(dir $@)
	nasm -f elf32 -o $@ $<

ifeq ($(DEBUG), 1)
$(KERNEL_BIN): $(CPP_OBJ) $(NASM_OBJ) $(DBG_SYMBOLS)
else
$(KERNEL_BIN): $(CPP_OBJ) $(NASM_OBJ)
endif
	$(LD) $(LD_FLAGS) $(KERNEL_OBJ) ${OTHER_OBJ} $(NASM_OBJ) -o $@.elf
	cp $@.elf $@.raw
#	truncate -s 5120 $@ <- old (for 10 sectors)
	objcopy -O binary $@.elf $@
	@echo "Current kernel size: `stat -L -c %s $@`"
	truncate -s 65536 $@

$(OS_IMAGE): $(BUILD_FOLDER) $(BOOTLOADER_BIN) $(KERNEL_BIN)
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $@

$(BUILD_FOLDER)/%.sym: $(BUILD_FOLDER)/%.o
	objcopy --only-keep-debug $< $@
	objcopy --strip-debug $<

clean: 
	rm -rf $(BUILD_FOLDER)

run: $(OS_IMAGE)
	qemu-system-i386 $(QEMU_FLAG_SERIAL) $< 
	make clean

gdb:
	make DEBUG=1
	qemu-system-i386 $(OS_IMAGE) -s -S &
	gdb -q -iex "set auto-load safe-path /"
	make clean
