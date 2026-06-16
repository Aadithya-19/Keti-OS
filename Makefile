ASM = nasm
CC = i686-linux-gnu-gcc
LD = ld
BUILD_DIR = build

all: run

# Assemble the boot loader
boot/loader.o: boot/loader.asm
	$(ASM) -f elf32 boot/loader.asm -o boot/loader.o

# Compile the C kernel
kernel/kernel.o: kernel/kernel.c
	$(CC) -m32 -ffreestanding -c kernel/kernel.c -o kernel/kernel.o

kernel/vga.o: kernel/vga.c
	$(CC) -m32 -ffreestanding -c kernel/vga.c -o kernel/vga.o

# Link everything into a kernel ELF
$(BUILD_DIR)/kernel.elf: boot/loader.o kernel/kernel.o kernel/vga.o
	mkdir -p $(BUILD_DIR)
	$(LD) -T link.ld -melf_i386 boot/loader.o kernel/kernel.o kernel/vga.o -o $(BUILD_DIR)/kernel.elf

# Build the bootable ISO
$(BUILD_DIR)/keti.iso: $(BUILD_DIR)/kernel.elf
	mkdir -p $(BUILD_DIR)/isodir/boot/grub
	cp $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/isodir/boot/kernel.elf
	cp boot/grub.cfg $(BUILD_DIR)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD_DIR)/keti.iso $(BUILD_DIR)/isodir



# Boot in QEMU
run: $(BUILD_DIR)/keti.iso
	qemu-system-i386 -cdrom $(BUILD_DIR)/keti.iso -boot d

clean:
	rm -rf $(BUILD_DIR)
	rm -f boot/loader.o kernel/kernel.o kernel/vga.o

.PHONY: all run clean