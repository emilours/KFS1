CP := cp
RM := rm -rf
MKDIR := mkdir -pv

BIN = kernel
CFG = grub.cfg
ISO_PATH := iso
ISO_NAME := the_chosen_one.iso
BOOT_PATH := $(ISO_PATH)/boot
GRUB_PATH := $(BOOT_PATH)/grub

.PHONY: all clean bootloader kernel linker iso run fclean re
all: bootloader kernel linker iso run
	@echo Tasks completed.

# Assembling the bootloader:
# - CPUs boot in real mode (16-bit) and need assembly code to initialize
# - The bootloader handles the transition from real mode to protected mode (32-bit)
# - Sets up critical data structures like the GDT (Global Descriptor Table)
# - C code cannot run until the CPU is properly initialized
# - Creates the entry point that BIOS/UEFI will jump to
bootloader: boot.asm
	nasm -f elf32 boot.asm -o boot.o

# Compiling the kernel:
# - Contains your actual kernel logic (memory management, process handling, etc.)
# -m32 ensures 32-bit compatibility with your bootloader
# -c creates an object file (not an executable) since it needs special linking
kernel: kernel.c
# 	-fno-exceptions et -fno-rtti: only for c++
	gcc -m32 -fno-builtin -fno-stack-protector -ffreestanding -nostdlib -nodefaultlibs -c kernel.c -o kernel.o

# Linking everything together:
# - Memory layout control: Kernels need to be loaded at specific memory addresses
# - Section ordering: Bootloader code must come first, kernel code after
# - No standard library: Regular executables expect libc, kernels run bare metal
# - Custom entry point: could be main() or wherever your bootloader jumps
linker: linker.ld boot.o kernel.o
	ld -m elf_i386 -T linker.ld -o kernel boot.o kernel.o

# Create a bootable ISO image using GRUB
iso: $(BIN)
	$(MKDIR) scripts/$(GRUB_PATH)
	$(CP) $(BIN) scripts/$(BOOT_PATH)
	$(CP) $(CFG) scripts/$(GRUB_PATH)
# Validates that the kernel binary is Multiboot-compliant
#	grub-file --is-x86-multiboot $(BOOT_PATH)/$(BIN)
	./scripts/check_multiboot.sh
# Creates a bootable ISO file that we can mount in a virtual machine (QEMU)
#	grub-mkrescue -o $(ISO_NAME) $(ISO_PATH)
# compose waits for the container to finish building before proceeding and
# --abort-on-container-exit stops all containers if one exits
	docker compose -f docker-compose.yaml up --build --abort-on-container-exit

run: iso
#	sleep 5
	$(CP) scripts/$(ISO_NAME) .
	qemu-system-i386 -cdrom $(ISO_NAME)

clean:
	$(RM) *.o $(BIN) *iso
	docker compose -f docker-compose.yaml down

fclean: clean
	$(RM) ./scripts/iso ./scripts/$(ISO_NAME)
	docker system prune -af

re: fclean all