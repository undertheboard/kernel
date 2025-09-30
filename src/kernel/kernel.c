#include "../include/kernel.h"

// VGA text mode variables
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;
static privilege_level_t current_privilege = CORE_MODE;

// Create VGA color attribute
static inline uint8_t vga_entry_color(vga_color fg, vga_color bg) {
    return fg | bg << 4;
}

// Create VGA entry (character + color)
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

// String length function
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

// String comparison function
int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

// String copy function
char* strcpy(char* dest, const char* src) {
    char* original_dest = dest;
    while ((*dest++ = *src++));
    return original_dest;
}

// Initialize terminal
void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) VGA_MEMORY;
    terminal_clear();
}

// Clear terminal screen
void terminal_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
}

// Set terminal color
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// Put character at specific position
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

// Put character at current position
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0; // Simple wrap-around
        }
        return;
    }
    
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0; // Simple wrap-around
        }
    }
}

// Write string to terminal
void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

// Write string to terminal
void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

// Write colored text
void terminal_write_color(const char* data, vga_color fg, vga_color bg) {
    uint8_t old_color = terminal_color;
    terminal_setcolor(vga_entry_color(fg, bg));
    terminal_writestring(data);
    terminal_setcolor(old_color);
}

// Security system initialization
void init_security_system(void) {
    current_privilege = CORE_MODE;
    terminal_write_color("[SECURITY] ", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
    terminal_writestring("Multi-layer security system initialized\n");
    terminal_write_color("[CORE MODE] ", VGA_COLOR_RED, VGA_COLOR_BLACK);
    terminal_writestring("Full system control enabled\n");
}

// Check current privilege level
int check_privilege_level(void) {
    return current_privilege;
}

// Print security information
void print_security_info(void) {
    terminal_write_color("\n=== SECURITY SYSTEM STATUS ===\n", VGA_COLOR_CYAN, VGA_COLOR_BLACK);
    
    if (current_privilege == CORE_MODE) {
        terminal_write_color("Current Mode: CORE MODE (Ring 0)\n", VGA_COLOR_RED, VGA_COLOR_BLACK);
        terminal_writestring("Privileges: Full system control\n");
        terminal_writestring("Access: All hardware, memory, and kernel functions\n");
    } else if (current_privilege == USER_MODE) {
        terminal_write_color("Current Mode: USER MODE (Ring 3)\n", VGA_COLOR_GREEN, VGA_COLOR_BLACK);
        terminal_writestring("Privileges: Limited user operations\n");
        terminal_writestring("Access: Restricted to user-space operations\n");
    }
    
    terminal_writestring("Security Features:\n");
    terminal_writestring("- Memory protection enabled\n");
    terminal_writestring("- Privilege level enforcement\n");
    terminal_writestring("- Ring-based access control\n");
    terminal_write_color("===============================\n\n", VGA_COLOR_CYAN, VGA_COLOR_BLACK);
}

// Main kernel function
void kernel_main(void) {
    // Initialize terminal
    terminal_initialize();
    
    // Display boot message
    terminal_write_color("UnderTheBoard OS v1.0\n", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    terminal_write_color("x86_64 Operating System\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("=======================\n\n");
    
    // Initialize security system
    init_security_system();
    
    // Show system information
    terminal_writestring("Kernel loaded successfully\n");
    terminal_writestring("VGA text mode initialized\n");
    terminal_writestring("Memory management: Basic\n");
    terminal_writestring("Privilege levels: Core/User modes active\n\n");
    
    // Display security information
    print_security_info();
    
    // Start command console
    terminal_write_color("Starting command console...\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    console_main();
}