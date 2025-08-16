DEBUG				?= 0

# Executables
GCC					= g++
LD					= ld
NASM				= nasm
OBJCOPY				= objcopy
MAKE				= make

# Files
LINKER_FILE			= linker.ld

# Folders
CURRENT_DIRECTORY   =  $(abspath $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
BUILD_FOLDER		=  ../bin
SRC_FOLDER			=  ./ 

$(info BUILD_FOLDER: $(BUILD_FOLDER))
$(info SRC_FOLDER: $(SRC_FOLDER))

# Flags
# for using __attribute__((interrupt)):
# -mgeneral-regs-only 
# -Werror
# -fno-stack-protector (stack overflows wont be found and it might find bugs if enabled)
GCC_FLAGS			=  -ffreestanding \
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
					   -Wno-div-by-zero \
					   -DLIMINE_API_REVISION=3 \
					   -mcmodel=kernel \
					   #-m64 \
					   -march=x86-64 \
					   -mno-red-zone \
					   -mno-mmx \
					   -mno-sse \
					   -mno-sse2 \
					   -mno-80387 \
					   -mabi=sysv

GCC_FLAGS_DBG		=  -g -D__DEBUG__

GCC_FLAGS_GDB		=  -D__GDB__

LD_FLAGS			=  -T $(LINKER_FILE) \
					   -m elf_x86_64 \
					   -nostdlib \
					   -static \
					   -z max-page-size=0x1000 \
					   --gc-sections

NASM_FLAGS			=  -Wall \
					   -f elf64 
						#-Werror \

# Source files
CPP_SRC				:= $(patsubst ./%,%,$(shell find -name '*.cpp'))
NASM_SRC		    := $(patsubst ./%,%,$(shell find -name '*.nasm'))
CPP_HEADERS			:= $(patsubst ./%,%,$(shell find -name '*.h'))

# Output files
KERNEL_BIN 	        =  $(BUILD_FOLDER)/kernel.bin
KERNEL_ELF			=  $(BUILD_FOLDER)/kernel.elf
KERNEL_RAW			=  $(BUILD_FOLDER)/kernel.raw
KERNEL_BIN_DBG 	    =  $(BUILD_FOLDER)/kernel.dbg.bin
KERNEL_SYM			=  $(BUILD_FOLDER)/kernel.sym

CPP_OBJ				:= $(patsubst %.cpp, $(BUILD_FOLDER)/%.o,$(CPP_SRC))
NASM_OBJ			:= $(patsubst %.nasm,$(BUILD_FOLDER)/%.o,$(NASM_SRC))

DBG_SYMBOLS			:= $(patsubst %.o,$(BUILD_FOLDER)/%.sym,$(CPP_OBJ))

KERNEL_OBJ			=  $(BUILD_FOLDER)/kernel/kernel.o
OTHER_OBJ			:= $(filter-out $(KERNEL_OBJ), $(CPP_OBJ)) $(NASM_OBJ)

ifeq ($(DEBUG), 1)
	GCC_FLAGS += $(GCC_FLAGS_DBG)
endif

.PHONY: all clean format debug

all: clean $(BUILD_FOLDER) $(KERNEL_BIN)

$(BUILD_FOLDER):
	mkdir -p $(BUILD_FOLDER)

$(BUILD_FOLDER)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(GCC) $(GCC_FLAGS) -I$(SRC_FOLDER) -c $< -o $@

$(BUILD_FOLDER)/%.o: %.nasm
	mkdir -p $(dir $@)
	$(NASM) $(NASM_FLAGS) -o $@ $<

ifeq ($(DEBUG), 1)
$(KERNEL_BIN): $(CPP_OBJ) $(NASM_OBJ) $(DBG_SYMBOLS)
else
$(KERNEL_BIN): $(CPP_OBJ) $(NASM_OBJ)
endif
	$(LD) $(LD_FLAGS) $(KERNEL_OBJ) ${OTHER_OBJ} -o $(KERNEL_ELF)
	cp $(KERNEL_ELF) $(KERNEL_RAW)
ifeq ($(DEBUG), 1)
	# cat $(DBG_SYMBOLS) > $(KERNEL_SYM) 
endif
	$(OBJCOPY) -O binary $(KERNEL_ELF) $@
	@echo "Current kernel size: `stat -L -c %s $@`"
	#truncate -s $(KERNEL_SIZE) $@

$(BUILD_FOLDER)/%.sym: $(BUILD_FOLDER)/%.o
	# $(OBJCOPY) --only-keep-debug $< $@
	# $(OBJCOPY) --strip-debug $<

clean: 
	rm -rf $(BUILD_FOLDER)

format:
	find $(SRC_FOLDER) -regex '.*\.\(cpp\|h\).*' | xargs -r clang-format -i

debug: 
	$(MAKE) DEBUG=1
