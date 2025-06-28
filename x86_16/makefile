arch=x86_16
target=i386-elf
build_folder=./build

gcc=${HOME}/cross/bin/$(target)-gcc 
ld=${HOME}/cross/bin/$(target)-ld 

check-build-dir:
	mkdir -p $(build_folder)

build-bootsector:
	nasm -f bin -o $(build_folder)/nos.bin ./boot/bootsector.nasm

build-bootloader:
	nasm -f bin -o $(build_folder)/bootloader.bin ./boot/bootloader.nasm

build-asmkernel:
	nasm -f bin -o $(build_folder)/kernel.bin ./kernel/test_kernel.nasm

build-ckernel:
	$(gcc) -c ./kernel/kernel.c -o $(build_folder)/kernel.o -ffreestanding -O2 -Wall -Wextra -Werror

link-ckernel: build-ckernel
	$(ld) --Ttext 0x8000 --oformat binary -o $(build_folder)/kernel.bin $(build_folder)/kernel.o 
	rm $(build_folder)/kernel.o

create-final-bin:
	cat $(build_folder)/bootloader.bin $(build_folder)/kernel.bin > $(build_folder)/nos.bin

clean:
	rm $(build_folder)/bootloader.bin
	rm $(build_folder)/kernel.bin

#build-testkernel:
#	nasm -f bin -o $(build_folder)/test_kernel.bin ./boot/test_kernel.nasm

#create-test-bin:
#	cat $(build_folder)/bootloader.bin $(build_folder)/test_kernel.bin > $(build_folder)/nos.bin

#create-test-img:
#	dd status=noxfer conv=notrunc if=$(build_folder)/nos.bin of=$(build_folder)/nos.flp

#build-kernel:
#	$(gcc) -c ./kernel/kernel.c -o $(build_folder)/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Werror

#create-img:
#	dd status=noxfer conv=notrunc if=$(build_folder)/bootloader.bin of=$(build_folder)/nos.flp

run:
#	qemu-system-i386 -fda $(build_folder)/nos.flp
	qemu-system-i386 $(build_folder)/nos.bin

build-sector: check-build-dir build-bootsector run

build-kernel-asm: check-build-dir build-bootloader build-asmkernel create-final-bin clean run
	rm $(build_folder)/nos.bin

build-kernel-c: check-build-dir build-bootloader link-ckernel create-final-bin clean run
	rm $(build_folder)/nos.bin