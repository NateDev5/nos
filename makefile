DEBUG ?= 0
TESTING ?= 0

OS_NAME := nos
ARCH := x86_64
BOOTLOADER := limine

QEMU ?= qemu-system-$(ARCH)
GDB ?= gdb
XORRISO ?= xorriso

BOOT_DIR := boot
SRC_DIR := src
BUILD_DIR := bin
ISO_DIR := iso
ISO_BOOT_DIR := $(ISO_DIR)/boot/limine

KERNEL := $(BUILD_DIR)/kernel.elf
ISO_KERNEL := $(ISO_DIR)/boot/kernel.elf

LIMINE_DIR := limine
LIMINE_CONF := $(BOOT_DIR)/limine/limine.conf
LIMINE_BIOS_SYS := $(LIMINE_DIR)/limine-bios.sys
LIMINE_BIOS_CD := $(LIMINE_DIR)/limine-bios-cd.bin
LIMINE_EXECUTABLE := $(LIMINE_DIR)/limine

ifeq ($(DEBUG),1)
OS_OUT := $(BUILD_DIR)/$(OS_NAME)_$(ARCH)_$(BOOTLOADER)_debug.iso
QEMU_RUN_FLAGS := -serial stdio
RUN_LABEL := Debug + Serial
else
OS_OUT := $(BUILD_DIR)/$(OS_NAME)_$(ARCH)_$(BOOTLOADER)_release.iso
QEMU_RUN_FLAGS :=
RUN_LABEL := Release
endif

QEMU_FLAGS := -M q35 -m 2G -no-reboot
QEMU_DBG_INT_FLAGS := -d int,cpu_reset -M smm=off
QEMU_MONITOR_FLAGS := -monitor stdio
XORRISO_FLAGS := -as mkisofs -R -r -J \
	-b boot/limine/limine-bios-cd.bin \
	-no-emul-boot -boot-load-size 4 -boot-info-table

.PHONY: all kernel run dbg monitor gdb int clean clean-all limine

all: $(OS_OUT)

limine: $(LIMINE_EXECUTABLE)

$(LIMINE_EXECUTABLE): $(LIMINE_DIR)/Makefile
	@printf '\n>>> Building Limine from submodule <<<\n\n'
	$(MAKE) -C $(LIMINE_DIR)

$(ISO_BOOT_DIR):
	mkdir -p $@

$(ISO_BOOT_DIR)/limine.conf: $(LIMINE_CONF) | $(ISO_BOOT_DIR)
	cp -v $< $@

$(ISO_BOOT_DIR)/limine-bios.sys: $(LIMINE_BIOS_SYS) | $(ISO_BOOT_DIR)
	cp -v $< $@

$(ISO_BOOT_DIR)/limine-bios-cd.bin: $(LIMINE_BIOS_CD) | $(ISO_BOOT_DIR)
	cp -v $< $@

$(ISO_KERNEL): kernel
	mkdir -p $(dir $@)
	cp -v $(KERNEL) $@

$(OS_OUT): $(LIMINE_EXECUTABLE) $(ISO_KERNEL) $(ISO_BOOT_DIR)/limine.conf $(ISO_BOOT_DIR)/limine-bios.sys $(ISO_BOOT_DIR)/limine-bios-cd.bin
	@printf '\n>>> Creating final OS image <<<\n\n'
	mkdir -p $(dir $@)
	$(XORRISO) $(XORRISO_FLAGS) $(ISO_DIR) -o $@
	$(LIMINE_EXECUTABLE) bios-install $@

kernel:
	@printf '\n>>> Building kernel <<<\n\n'
	$(MAKE) -C $(SRC_DIR) DEBUG=$(DEBUG) TESTING=$(TESTING)

run:
	$(MAKE) DEBUG=$(DEBUG) TESTING=$(TESTING)
	@printf '\n>>> Running using QEMU ($(RUN_LABEL)) <<<\n\n'
	$(QEMU) $(QEMU_FLAGS) $(QEMU_RUN_FLAGS) -cdrom $(OS_OUT)

dbg:
	$(MAKE) run DEBUG=1 TESTING=1

monitor:
	$(MAKE) DEBUG=1 TESTING=$(TESTING)
	@printf '\n>>> Running using QEMU (Debug + Monitor) <<<\n\n'
	$(QEMU) $(QEMU_FLAGS) $(QEMU_MONITOR_FLAGS) -cdrom $(BUILD_DIR)/$(OS_NAME)_$(ARCH)_$(BOOTLOADER)_debug.iso

gdb:
	$(MAKE) DEBUG=1 TESTING=$(TESTING)
	@printf '\n>>> Debugging using QEMU/GDB (Debug + Serial) <<<\n\n'
	$(QEMU) $(QEMU_FLAGS) -cdrom $(BUILD_DIR)/$(OS_NAME)_$(ARCH)_$(BOOTLOADER)_debug.iso -s -S > qemu.log 2>&1 &
	#$(GDB) bin/kernel.elf -ex "target remote localhost:1234"
	$(GDB) -tui -q -iex "set auto-load safe-path /"

int:
	$(MAKE) DEBUG=1 TESTING=$(TESTING)
	@printf '\n>>> Running using QEMU (Debug + Interrupts) <<<\n\n'
	$(QEMU) $(QEMU_FLAGS) $(QEMU_DBG_INT_FLAGS) -cdrom $(BUILD_DIR)/$(OS_NAME)_$(ARCH)_$(BOOTLOADER)_debug.iso

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)

clean-all: clean
	$(MAKE) -C $(LIMINE_DIR) clean
