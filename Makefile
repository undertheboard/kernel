# UnderTheBoard OS Makefile
# Builds a complete x86_64 operating system

# Compiler and assembler settings
CC = gcc
AS = nasm
LD = ld

# Compiler flags
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector -m32
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -nostdlib

# Directories
SRCDIR = src
BUILDDIR = build
INCLUDEDIR = include

# Source files
BOOT_SRC = $(SRCDIR)/boot/boot.s
KERNEL_SRC = $(SRCDIR)/kernel/kernel.c $(SRCDIR)/kernel/console.c
GAME_SRC = $(SRCDIR)/games/adventure.c

# Object files
BOOT_OBJ = $(BUILDDIR)/boot.o
KERNEL_OBJ = $(BUILDDIR)/kernel.o $(BUILDDIR)/console.o
GAME_OBJ = $(BUILDDIR)/adventure.o

ALL_OBJS = $(BOOT_OBJ) $(KERNEL_OBJ) $(GAME_OBJ)

# Target binary
KERNEL_BIN = $(BUILDDIR)/kernel.bin
ISO_FILE = $(BUILDDIR)/undertheboard-os.iso

# Default target
all: $(ISO_FILE)

# Create build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Compile assembly files
$(BUILDDIR)/%.o: $(SRCDIR)/boot/%.s | $(BUILDDIR)
	$(AS) $(ASFLAGS) $< -o $@

# Compile C files - kernel
$(BUILDDIR)/kernel.o: $(SRCDIR)/kernel/kernel.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -c $< -o $@

$(BUILDDIR)/console.o: $(SRCDIR)/kernel/console.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -c $< -o $@

# Compile C files - games
$(BUILDDIR)/adventure.o: $(SRCDIR)/games/adventure.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -c $< -o $@

# Link kernel
$(KERNEL_BIN): $(ALL_OBJS) $(SRCDIR)/linker.ld | $(BUILDDIR)
	$(LD) $(LDFLAGS) -T $(SRCDIR)/linker.ld -o $@ $(ALL_OBJS)

# Verify multiboot compliance
verify: $(KERNEL_BIN)
	@echo "Checking multiboot compliance..."
	@if command -v grub-file >/dev/null 2>&1; then \
		grub-file --is-x86-multiboot $(KERNEL_BIN) && echo "✓ Multiboot compliant" || echo "✗ Not multiboot compliant"; \
	else \
		echo "Note: grub-file not available, skipping multiboot verification"; \
	fi

# Create GRUB configuration
$(BUILDDIR)/grub.cfg: | $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/iso/boot/grub
	@echo 'menuentry "UnderTheBoard OS" {' > $(BUILDDIR)/iso/boot/grub/grub.cfg
	@echo '    multiboot /boot/kernel.bin' >> $(BUILDDIR)/iso/boot/grub/grub.cfg
	@echo '}' >> $(BUILDDIR)/iso/boot/grub/grub.cfg

# Create ISO image
$(ISO_FILE): $(KERNEL_BIN) $(BUILDDIR)/grub.cfg | $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/iso/boot
	@cp $(KERNEL_BIN) $(BUILDDIR)/iso/boot/kernel.bin
	@if command -v grub-mkrescue >/dev/null 2>&1; then \
		grub-mkrescue -o $(ISO_FILE) $(BUILDDIR)/iso 2>/dev/null || \
		echo "Warning: grub-mkrescue failed, ISO not created"; \
	else \
		echo "Note: grub-mkrescue not available, creating basic structure only"; \
		echo "Kernel binary available at: $(KERNEL_BIN)"; \
	fi

# Test with QEMU (if available)
test: $(ISO_FILE)
	@if command -v qemu-system-i386 >/dev/null 2>&1; then \
		echo "Starting QEMU (press Ctrl+Alt+G to release mouse, Ctrl+Alt+Q to quit)..."; \
		qemu-system-i386 -cdrom $(ISO_FILE); \
	else \
		echo "QEMU not available. To test manually:"; \
		echo "1. Use VirtualBox, VMware, or other VM software"; \
		echo "2. Boot from $(ISO_FILE)"; \
		echo "3. Or use: qemu-system-i386 -cdrom $(ISO_FILE)"; \
	fi

# Test kernel directly (without ISO)
test-kernel: $(KERNEL_BIN)
	@if command -v qemu-system-i386 >/dev/null 2>&1; then \
		echo "Testing kernel directly with QEMU..."; \
		qemu-system-i386 -kernel $(KERNEL_BIN); \
	else \
		echo "QEMU not available for direct kernel testing"; \
	fi

# Clean build files
clean:
	rm -rf $(BUILDDIR)

# Show build information
info:
	@echo "UnderTheBoard OS Build System"
	@echo "============================="
	@echo "Compiler: $(CC)"
	@echo "Assembler: $(AS)"
	@echo "Linker: $(LD)"
	@echo "Target: x86 32-bit (multiboot)"
	@echo "Output: $(KERNEL_BIN)"
	@echo "ISO: $(ISO_FILE)"
	@echo ""
	@echo "Available targets:"
	@echo "  all      - Build complete OS"
	@echo "  verify   - Check multiboot compliance"
	@echo "  test     - Run in QEMU (if available)"
	@echo "  clean    - Clean build files"
	@echo "  info     - Show this information"

# Help target
help: info

.PHONY: all clean test test-kernel verify info help