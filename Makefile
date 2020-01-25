AS=i686-elf-as
CC=i686-elf-gcc
CXX=i686-elf-g++
CXXFLAGS=-ffreestanding -O2 -Wall -Wextra -Werror -std=c++2a

myos.iso: myos.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

myos.bin: boot.o kernel.o linker.ld
	i686-elf-g++ -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o

.PHONY: clean
clean:
	rm -rf *o myos.bin myos.iso isodir/
