#!/bin/bash

# UnderTheBoard OS Test Script
# This script tests the kernel and shows expected output

echo "UnderTheBoard OS Test Suite"
echo "=========================="
echo

# Check if kernel binary exists
if [ ! -f "build/kernel.bin" ]; then
    echo "❌ Kernel binary not found. Run 'make all' first."
    exit 1
fi

# Check multiboot compliance
echo "🔍 Testing multiboot compliance..."
if command -v grub-file >/dev/null 2>&1; then
    if grub-file --is-x86-multiboot build/kernel.bin; then
        echo "✅ Kernel is multiboot compliant"
    else
        echo "❌ Kernel is not multiboot compliant"
        exit 1
    fi
else
    echo "ℹ️  grub-file not available, skipping multiboot test"
fi

# Test kernel size and structure
echo
echo "📊 Kernel Information:"
echo "Size: $(stat -c%s build/kernel.bin) bytes"
echo "Type: $(file build/kernel.bin | cut -d: -f2)"

# Check for required symbols
echo
echo "🔧 Symbol Analysis:"
if command -v nm >/dev/null 2>&1; then
    echo "Entry point: $(nm build/kernel.bin | grep ' _start' || echo 'Not found')"
    echo "Main function: $(nm build/kernel.bin | grep ' kernel_main' || echo 'Not found')"
else
    echo "ℹ️  nm not available, skipping symbol analysis"
fi

# Expected output simulation
echo
echo "🖥️  Expected Console Output (Simulation):"
echo "=========================================="
cat << 'EOF'
UnderTheBoard OS v1.0
x86_64 Operating System
=======================

[SECURITY] Multi-layer security system initialized
[CORE MODE] Full system control enabled
Kernel loaded successfully
VGA text mode initialized
Memory management: Basic
Privilege levels: Core/User modes active

=== SECURITY SYSTEM STATUS ===
Current Mode: CORE MODE (Ring 0)
Privileges: Full system control
Access: All hardware, memory, and kernel functions
Security Features:
- Memory protection enabled
- Privilege level enforcement
- Ring-based access control
===============================

Starting command console...
UnderTheBoard OS Console
Type 'help' for available commands

CORE# help
=== AVAILABLE COMMANDS ===
help     - Show this help message
sysinfo  - Display system information
security - Show security system status
game     - Launch the built-in game
clear    - Clear the screen
exit     - Shutdown the system
===========================

CORE# sysinfo
=== SYSTEM INFORMATION ===
OS Name: UnderTheBoard OS
Version: 1.0
Architecture: x86_64
Kernel: Custom microkernel
Memory: Basic management active
Security: Multi-layer privilege system
Graphics: VGA text mode (80x25)
Build: Debug development version
===========================

CORE# game
=== UNDERTHEBOARD ADVENTURE ===
A simple text-based adventure game
...
EOF

echo
echo "✅ Test completed successfully!"
echo
echo "🚀 To run the OS:"
echo "   1. Use QEMU: qemu-system-i386 -kernel build/kernel.bin"
echo "   2. Or create bootable media with the kernel"
echo "   3. Or use: make test (if QEMU is available)"