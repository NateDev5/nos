DEBUG ?= 0
TESTING ?= 0

CXX ?= g++
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
CXX_FLAGS := -ffreestanding \
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
	-DLIMINE_API_REVISION=3 \
	-mcmodel=kernel \
	-m64 \
	-march=x86-64 \
	-mno-red-zone \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-80387 \
	-mabi=sysv \
	-MMD \
	-MP

CXX_FLAGS_DEBUG := -g -D__DEBUG__
CXX_FLAGS_TESTING := -D__TESTING__

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
CXX_FLAGS += $(CXX_FLAGS_DEBUG)
endif

ifeq ($(TESTING), 1)
CXX_FLAGS += $(CXX_FLAGS_TESTING)
endif

.PHONY: all clean format debug

all: $(KERNEL_BIN)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) -I$(SRC_DIR) -c $< -o $@

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
