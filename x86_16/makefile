arch=x86_16
target=i386-elf
build_folder=./build

gcc=${HOME}/cross/bin/$(target)-gcc 

check-build-dir:
	mkdir -p $(build_folder)

build-bootloader:
	nasm -f bin -o $(build_folder)/bootloader-$(arch).bin ./boot/bootloader-$(arch).nasm

build-kernel:
	$(gcc) -c ./kernel/kernel.c -o $(build_folder)/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Werror

create-img:
	dd status=noxfer conv=notrunc if=$(build_folder)/bootloader-$(arch).bin of=$(build_folder)/bootloader-$(arch).flp

run:
	qemu-system-i386 -fda $(build_folder)/bootloader-$(arch).flp

build: check-build-dir build-bootloader create-img run