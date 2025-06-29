arch=x86_32
target=i386-elf
build_folder=./build

gcc=${HOME}/cross/bin/$(target)-gcc 
ld=${HOME}/cross/bin/$(target)-ld

check-build-dir:
	mkdir -p $(build_folder)

build-bootloader:
	nasm -f bin -o $(build_folder)/bootloader.bin ./boot/bootloader.nasm

build-kernel:
	$(gcc) -c ./kernel/kernel.c -o $(build_folder)/kernel.o -ffreestanding -O2 -Wall -Wextra -Werror

link-kernel: build-kernel
	$(ld) --Ttext 0x8000 --oformat binary -o $(build_folder)/kernel.bin $(build_folder)/kernel.o 
	rm $(build_folder)/kernel.o

create-final-bin:
	cat $(build_folder)/bootloader.bin $(build_folder)/kernel.bin > $(build_folder)/nos.bin

run:
	qemu-system-i386 $(build_folder)/nos.bin

build: check-build-dir build-bootloader link-kernel create-final-bin run