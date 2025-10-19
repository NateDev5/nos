DEBUG ?= 0
TESTING ?= 0

# Info
OS_NAME				= nos
ARCH				= x86_64
BOOTLOADER			= limine

# Executables
MAKE				= make
QEMU				= qemu-system-$(ARCH)
GDB					= gdb
GIT					= git
XORRISO				= xorriso

# Folders
BOOT_FOLDER			= boot
SRC_FOLDER			= src
SRC_BOOT_FOLDER		= $(SRC_FOLDER)/boot
BUILD_FOLDER		= bin
ISO_FOLDER			= iso

# Files
KERNEL				= $(BUILD_FOLDER)/kernel.elf

# Bootloader
LIMINE_REPO			= https://github.com/limine-bootloader/limine.git
LIMINE_VERSION		= v9.x-binary
LIMINE_FOLDER		= limine_bin
LIMINE_CONF			= $(BOOT_FOLDER)/limine/limine.conf
LIMINE_BIOS_SYS		= $(LIMINE_FOLDER)/limine-bios.sys
LIMINE_BIOS_CD		= $(LIMINE_FOLDER)/limine-bios-cd.bin
LIMINE_HEADER		= $(LIMINE_FOLDER)/limine.h
LIMINE_EXECUTABLE	= $(LIMINE_FOLDER)/limine

# Output files
ifeq ($(DEBUG),1)
	OS_OUT			:= $(BUILD_FOLDER)/$(OS_NAME)_$(ARCH)_$(BOOTLOADER)_debug.iso
else
	OS_OUT			:= $(BUILD_FOLDER)/$(OS_NAME)_$(ARCH)_$(BOOTLOADER)_release.iso
endif

# Flags
QEMU_FLAGS			= -M q35 -m 8G -no-reboot
QEMU_FLAG_DBG_INT	= -d int,cpu_reset
QEMU_FLAG_SERIAL	= -serial stdio
QEMU_FLAG_MONITOR	= -monitor stdio

XORRISO_FLAGS		= -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table

.PHONY: all kernel run dbg monitor gdb clean clean-all

all: $(OS_OUT)

$(LIMINE_FOLDER):
	@echo -e "\n>>> Cloning and building limine binaries <<<\n"
	$(GIT) clone $(LIMINE_REPO) --branch=$(LIMINE_VERSION) --depth=1 $(LIMINE_FOLDER)
	$(MAKE) -C $(LIMINE_FOLDER)
	cp -rv $(LIMINE_HEADER) $(SRC_BOOT_FOLDER)/limine

$(ISO_FOLDER):
	mkdir -pv $@/boot/limine
	cp -v $(LIMINE_CONF) $(LIMINE_BIOS_SYS) $(LIMINE_BIOS_CD) $@/boot/limine

$(OS_OUT): $(LIMINE_FOLDER) $(ISO_FOLDER)
	$(MAKE) kernel DEBUG=$(DEBUG) TESTING=$(TESTING)
	@echo -e "\n>>> Creating final os binaries <<<\n"
	cp -v $(KERNEL) $(ISO_FOLDER)/boot
	$(XORRISO) $(XORRISO_FLAGS) $(ISO_FOLDER) -o $@
	$(LIMINE_EXECUTABLE) bios-install $@

kernel:
	@echo -e "\n>>> Building kernel <<<\n"
	$(MAKE) -C $(SRC_FOLDER)

ifeq ($(DEBUG),1)
run: 
	$(MAKE) DEBUG=1 TESTING=$(TESTING)
	@echo -e "\n>>> Running using QEMU (Debug + Serial) <<<\n"
	$(QEMU) $(QEMU_FLAGS) $(QEMU_FLAG_SERIAL) -cdrom $(OS_OUT)

monitor:
	$(MAKE) DEBUG=1
	@echo -e "\n>>> Running using QEMU (Debug + Monitor) <<<\n"
	$(QEMU) $(QEMU_FLAGS) $(QEMU_FLAG_MONITOR) -cdrom $(OS_OUT)

gdb:
	$(MAKE) DEBUG=1
	@echo -e "\n>>> Debugging using QEMU/GDB (Debug + Serial)"
	$(QEMU) $(QEMU_FLAGS) $(QEMU_FLAG_SERIAL) -cdrom $(OS_OUT) -s -S & > qemu.log
	$(GDB) -tui -q -iex "set auto-load safe-path /"

int:
	$(MAKE) DEBUG=1
	@echo -e "\n>>> Running using QEMU (Debug + Interrupts)"
	$(QEMU) $(QEMU_FLAGS) $(QEMU_FLAG_DBG_INT) -cdrom $(OS_OUT)

else
run:
	$(MAKE) DEBUG=0
	@echo -e "\n>>> Running using QEMU (Release) <<\n"
	$(QEMU) $(QEMU_FLAGS) -cdrom $(OS_OUT)
endif

dbg:
	$(MAKE) run DEBUG=1 TESTING=1
	
clean:
	rm -rf $(BUILD_FOLDER) $(ISO_FOLDER)

clean-all:
	rm -rf $(BUILD_FOLDER) $(LIMINE_FOLDER) $(ISO_FOLDER)
