if grub-file --is-x86-multiboot scripts/iso/boot/kernel; then
	echo multiboot confirmed
else
	echo the file is not multiboot
fi