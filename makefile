build-boot-16:
	nasm -f bin -o ./build/bootsector-16bit.bin ./boot/bootsector-16bit.nasm
	dd status=noxfer conv=notrunc if=./build/bootsector-16bit.bin of=./build/bootsector-16bit.flp
	qemu-system-i386 -fda ./build/bootsector-16bit.flp