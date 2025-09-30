# UnderTheBoard OS

A full-fledged x86_64 operating system with multi-layer security, command console, and built-in game.

## Features

### Core OS Components
- **Multiboot-compliant bootloader** - GRUB compatible boot process
- **x86_64 kernel** - Custom kernel written in C and Assembly
- **VGA text mode display** - 80x25 color text interface
- **Multi-layer security system** - Ring-based privilege levels (Core/Kernel/User modes)
- **Memory management** - Basic memory layout and protection
- **Command console** - Interactive shell interface

### Security System
- **Core Mode (Ring 0)** - Complete system control with full hardware access
- **Kernel Mode (Ring 0)** - Kernel operations with privileged access
- **User Mode (Ring 3)** - Restricted user-space operations
- **Memory protection** - Segmentation and basic access control
- **Privilege enforcement** - Ring-based security model

### User Interface
- **Interactive console** - Command-line interface with multiple commands
- **System information** - Hardware and OS status reporting
- **Security status** - Real-time privilege level monitoring
- **Help system** - Built-in documentation

### Built-in Game
- **UnderTheBoard Adventure** - Text-based exploration game
- **ASCII graphics** - Character-based game field
- **Score system** - Points and level tracking
- **Interactive gameplay** - Movement and treasure collection

## Architecture

```
UnderTheBoard OS
├── Boot Loader (Assembly)
│   ├── Multiboot header
│   ├── Stack setup
│   └── Kernel entry point
├── Kernel (C + Assembly)
│   ├── VGA text mode driver
│   ├── Memory management
│   ├── Security system
│   └── System calls
├── Console (C)
│   ├── Command parser
│   ├── Built-in commands
│   └── User interface
└── Games (C)
    └── Adventure game
```

## Building

### Prerequisites
- GCC (GNU Compiler Collection)
- NASM (Netwide Assembler)
- GNU Binutils (ld, objcopy, etc.)
- GRUB tools (optional, for ISO creation)
- QEMU (optional, for testing)

### Compilation
```bash
# Build the complete OS
make all

# Check multiboot compliance
make verify

# Show build information
make info

# Clean build files
make clean
```

### Testing
```bash
# Test with QEMU (if available)
make test

# Test kernel directly
make test-kernel
```

## Security Model

The OS implements a multi-layer security architecture:

### Privilege Levels
1. **Core Mode (Ring 0)**
   - Full system control
   - Direct hardware access
   - Memory management
   - Interrupt handling

2. **User Mode (Ring 3)**
   - Limited application execution
   - Restricted memory access
   - System call interface
   - Protected operations

### Memory Protection
- Segmented memory model
- Ring-based access control
- Stack protection
- Basic heap management

## Console Commands

| Command | Description |
|---------|-------------|
| `help` | Display available commands |
| `sysinfo` | Show system information |
| `security` | Display security system status |
| `game` | Launch UnderTheBoard Adventure |
| `clear` | Clear the screen |
| `exit` | Shutdown the system |

## Game: UnderTheBoard Adventure

A simple text-based adventure game featuring:
- ASCII maze navigation
- Treasure collection
- Score tracking
- Real-time gameplay
- Character-based graphics

### Game Controls
- `W` - Move up
- `S` - Move down  
- `A` - Move left
- `D` - Move right
- `Q` - Quit game

### Game Elements
- `@` - Player character
- `$` - Treasure (100 points each)
- `#` - Walls and obstacles
- `.` - Empty walkable space

## File Structure

```
kernel/
├── src/
│   ├── boot/
│   │   └── boot.s          # Assembly bootloader
│   ├── kernel/
│   │   ├── kernel.c        # Main kernel code
│   │   └── console.c       # Console interface
│   ├── games/
│   │   └── adventure.c     # Built-in game
│   └── linker.ld          # Linker script
├── include/
│   └── kernel.h           # Header definitions
├── build/                 # Compiled objects (generated)
├── Makefile              # Build configuration
└── README.md             # This file
```

## Technical Details

### Bootloader
- Multiboot specification compliance
- GRUB compatibility
- 32-bit x86 assembly
- Stack initialization
- Kernel handoff

### Kernel
- Written in C99 with GNU extensions
- VGA text mode (0xB8000 memory mapping)
- Interrupt management framework
- Basic memory management
- Security privilege enforcement

### Display System
- 80x25 character text mode
- 16-color palette support
- Direct VGA memory access
- Formatted text output
- Color-coded system messages

## Development Notes

This OS is designed as a learning project and demonstration of:
- Low-level system programming
- x86 architecture concepts
- Operating system design principles
- Security model implementation
- Hardware abstraction layers

### Limitations
- 32-bit mode only (though designed for x86_64 systems)
- No advanced memory management (paging)
- Simplified interrupt handling
- Basic device drivers only
- No networking or file system

### Future Enhancements
- 64-bit long mode support
- Advanced memory management (paging, virtual memory)
- Full interrupt descriptor table (IDT)
- Device driver framework
- File system implementation
- Network stack
- More sophisticated games and applications

## License

This project is part of the UnderTheBoard kernel repository and follows the repository's licensing terms.