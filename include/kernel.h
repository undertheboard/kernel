#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>

// VGA text mode constants
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// Security privilege levels
typedef enum {
    CORE_MODE = 0,      // Ring 0 - Complete system control
    KERNEL_MODE = 0,    // Ring 0 - Kernel operations
    USER_MODE = 3       // Ring 3 - User applications
} privilege_level_t;

// VGA colors
typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW = 14,
    VGA_COLOR_WHITE = 15,
} vga_color;

// Function prototypes
void kernel_main(void);
void terminal_initialize(void);
void terminal_clear(void);
void terminal_putchar(char c);
void terminal_writestring(const char* data);
void terminal_write_color(const char* data, vga_color fg, vga_color bg);
void print_security_info(void);
void init_security_system(void);
void console_main(void);
void launch_game(void);

// Security system functions
int check_privilege_level(void);
void switch_to_core_mode(void);
void switch_to_user_mode(void);

// String utilities
size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);
char* strcpy(char* dest, const char* src);

#endif