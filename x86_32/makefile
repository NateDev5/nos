DEBUG				?= 0

# Executables
GCC					= g++
LD					= ld
NASM				= nasm
OBJCOPY				= objcopy
MAKE				= make
QEMU				= qemu-system-i386
GDB					= gdb

# Folders
BOOT_FOLDER			=  boot
BUILD_FOLDER		=  bin
SRC_FOLDER			=  src
KERNEL_FOLDER		=  $(SRC_FOLDER)/kernel

# Flags
# for using __attribute__((interrupt)):
# -mgeneral-regs-only 
# -Werror
# -fno-stack-protector (stack overflows wont be found and it might find bugs if enabled)
GCC_FLAGS			=  -m32 -ffreestanding -fno-pic -fno-pie -fno-exceptions -fno-rtti -fno-stack-protector -Werror -Wall -mgeneral-regs-only
GCC_FLAGS_DBG		=  -g -D__DEBUG__
GCC_FLAGS_GDB		=  -D__GDB__
LD_FLAGS			=  -melf_i386 -Ttext 0x9400 -e kmain -nostdlib --oformat elf32-i386

# debug interrupts
QEMU_FLAG_DBG_INT	=  -d int
QEMU_FLAG_SERIAL	=  -serial stdio

# Source files
CPP_SRC				:= $(shell find $(SRC_FOLDER) -name '*.cpp')
NASM_SRC		    := $(shell find $(SRC_FOLDER) -name '*.nasm')
CPP_HEADERS			:= $(shell find $(SRC_FOLDER) -name '*.h')

# Output files
BOOTLOADER_BIN 	    =  $(BUILD_FOLDER)/bootloader/bootloader.bin
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

# Other
KERNEL_SIZE			=  59904 # 65,536 - (512 + 5120) || 64KB - (Bootsector size + Bootloader stage 2 size)

ifeq ($(DEBUG), 1)
	GCC_FLAGS += $(GCC_FLAGS_DBG)
endif

.PHONY: all clean run run-dbg format gdb

all: $(OS_IMAGE)

$(BUILD_FOLDER):
	mkdir -p $(BUILD_FOLDER)

$(BOOTLOADER_BIN):
	$(MAKE) -C $(BOOT_FOLDER)

$(BUILD_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp
	mkdir -p $(dir $@)
	$(GCC) $(GCC_FLAGS) -I$(SRC_FOLDER) -c $< -o $@

$(BUILD_FOLDER)/%.o: $(SRC_FOLDER)/%.nasm
	mkdir -p $(dir $@)
	$(NASM) -f elf32 -o $@ $<

ifeq ($(DEBUG), 1)
$(KERNEL_BIN): $(CPP_OBJ) $(NASM_OBJ) $(DBG_SYMBOLS)
else
$(KERNEL_BIN): $(CPP_OBJ) $(NASM_OBJ)
endif
	$(LD) $(LD_FLAGS) $(KERNEL_OBJ) ${OTHER_OBJ} $(NASM_OBJ) -o $@.elf
	cp $@.elf $@.raw
	$(OBJCOPY) -O binary $@.elf $@
	@echo "Current kernel size: `stat -L -c %s $@`"
	truncate -s $(KERNEL_SIZE) $@

$(OS_IMAGE): $(BUILD_FOLDER) $(BOOTLOADER_BIN) $(KERNEL_BIN)
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $@

$(BUILD_FOLDER)/%.sym: $(BUILD_FOLDER)/%.o
	$(OBJCOPY) --only-keep-debug $< $@
	$(OBJCOPY) --strip-debug $<

clean: 
	rm -rf $(BUILD_FOLDER)

format:
	find $(SRC_FOLDER) -regex '.*\.\(cpp\|h\).*' | xargs -r clang-format -i

run: $(OS_IMAGE)
	$(QEMU) $(QEMU_FLAG_SERIAL) $< 
	$(MAKE) clean

run-dbg:
	$(MAKE) run DEBUG=1

gdb:
	$(MAKE) DEBUG=1
	$(QEMU) $(OS_IMAGE) -s -S &
	$(GDB) -q -iex "set auto-load safe-path /"
	$(MAKE) clean
