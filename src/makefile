DEBUG ?= 0
TESTING ?= 0

C_COMPILER ?= clang
LD ?= ld
NASM ?= nasm
OBJCOPY ?= objcopy

LINKER_FILE := linker.ld
BUILD_DIR := ../bin
SRC_DIR := .

# for using __attribute__((interrupt)):
# -mgeneral-regs-only 
# -Werror
# -fno-stack-protector (stack overflows wont be found and it might find bugs if enabled)
C_COMPILER_FLAGS := -ffreestanding \
	-std=c++20 \
	-fno-pic \
	-fno-pie \
	-fno-exceptions \
	-fno-rtti \
	-fno-stack-protector \
	-fno-stack-check \
	-ffunction-sections \
	-fdata-sections \
	-Werror \
	-Wall \
	-DLIMINE_API_REVISION=5 \
	-mcmodel=kernel \
	-m64 \
	-march=x86-64 \
	-target x86_64-elf \
	-mno-red-zone \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-80387 \
	-mabi=sysv \
	-MMD \
	-MP

C_COMPILER_FLAGS_DEBUG := -g -D__DEBUG__
C_COMPILER_FLAGS_TESTING := -D__TESTING__

LD_FLAGS := -T $(LINKER_FILE) \
	-m elf_x86_64 \
	-nostdlib \
	-static \
	-z max-page-size=0x1000 \
	--gc-sections

NASM_FLAGS := -Wall -f elf64

# Source files
CPP_SRC := $(patsubst ./%,%,$(shell find . -name '*.cpp'))
NASM_SRC := $(patsubst ./%,%,$(shell find . -name '*.nasm'))

# Output files
KERNEL_BIN := $(BUILD_DIR)/kernel.bin
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
KERNEL_RAW := $(BUILD_DIR)/kernel.raw

CPP_OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CPP_SRC))
NASM_OBJ := $(patsubst %.nasm,$(BUILD_DIR)/%.o,$(NASM_SRC))
DEPFILES := $(CPP_OBJ:.o=.d)

KERNEL_OBJ := $(BUILD_DIR)/kernel/kernel.o
OTHER_OBJ := $(filter-out $(KERNEL_OBJ),$(CPP_OBJ)) $(NASM_OBJ)

ifeq ($(DEBUG), 1)
C_COMPILER_FLAGS += $(C_COMPILER_FLAGS_DEBUG)
endif

ifeq ($(TESTING), 1)
C_COMPILER_FLAGS += $(C_COMPILER_FLAGS_TESTING)
endif

.PHONY: all clean format debug

all: $(KERNEL_BIN)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(C_COMPILER) $(C_COMPILER_FLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/%.o: %.nasm
	mkdir -p $(dir $@)
	$(NASM) $(NASM_FLAGS) -o $@ $<

$(KERNEL_BIN): $(CPP_OBJ) $(NASM_OBJ)
	$(LD) $(LD_FLAGS) $(KERNEL_OBJ) $(OTHER_OBJ) -o $(KERNEL_ELF)
	cp $(KERNEL_ELF) $(KERNEL_RAW)
	$(OBJCOPY) -O binary $(KERNEL_ELF) $@
	@echo "Current kernel size: `stat -L -c %s $@`"

clean: 
	rm -rf $(BUILD_DIR)

format:
	find $(SRC_DIR) -regex '.*\.\(cpp\|h\).*' | xargs -r clang-format -i

debug: 
	$(MAKE) DEBUG=1

-include $(DEPFILES)
