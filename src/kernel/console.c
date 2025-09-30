#include "../include/kernel.h"

// Console variables
static char command_buffer[256];
static size_t buffer_pos = 0;

// Forward declarations
void execute_command(const char* command);
void show_help(void);
void show_system_info(void);
void launch_game(void);
void handle_security_command(const char* args);

// Console prompt
void print_prompt(void) {
    if (check_privilege_level() == CORE_MODE) {
        terminal_write_color("CORE# ", VGA_COLOR_RED, VGA_COLOR_BLACK);
    } else {
        terminal_write_color("USER$ ", VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    }
}

// Simple keyboard input simulation (in a real OS, this would be interrupt-driven)
char get_char(void) {
    // This is a simplified version - in a real OS, we'd read from keyboard controller
    // For now, we'll simulate some basic commands
    static int command_index = 0;
    static const char* demo_commands[] = {
        "help\n",
        "sysinfo\n", 
        "security\n",
        "game\n",
        "exit\n"
    };
    
    if (command_index < 5) {
        const char* cmd = demo_commands[command_index];
        static int char_index = 0;
        
        if (cmd[char_index] == '\0') {
            command_index++;
            char_index = 0;
            return '\n';
        }
        
        char c = cmd[char_index++];
        if (cmd[char_index] == '\0') {
            command_index++;
            char_index = 0;
        }
        return c;
    }
    
    return 0; // No more input
}

// Process console input
void console_main(void) {
    terminal_writestring("UnderTheBoard OS Console\n");
    terminal_writestring("Type 'help' for available commands\n\n");
    
    while (1) {
        print_prompt();
        
        // Read command
        buffer_pos = 0;
        char c;
        
        // Simulate reading a few demo commands
        while ((c = get_char()) != 0) {
            if (c == '\n') {
                terminal_putchar('\n');
                command_buffer[buffer_pos] = '\0';
                
                if (buffer_pos > 0) {
                    execute_command(command_buffer);
                }
                break;
            } else if (c == '\b' && buffer_pos > 0) {
                // Backspace
                buffer_pos--;
                terminal_putchar('\b');
                terminal_putchar(' ');
                terminal_putchar('\b');
            } else if (buffer_pos < sizeof(command_buffer) - 1) {
                command_buffer[buffer_pos++] = c;
                terminal_putchar(c);
            }
        }
        
        // If no more input, break the demo loop
        if (c == 0) {
            terminal_writestring("\nDemo commands completed. System halted.\n");
            break;
        }
    }
}

// Execute command
void execute_command(const char* command) {
    if (strcmp(command, "help") == 0) {
        show_help();
    } else if (strcmp(command, "sysinfo") == 0) {
        show_system_info();
    } else if (strcmp(command, "security") == 0) {
        handle_security_command("");
    } else if (strcmp(command, "game") == 0) {
        launch_game();
    } else if (strcmp(command, "clear") == 0) {
        terminal_clear();
    } else if (strcmp(command, "exit") == 0) {
        terminal_write_color("Shutting down system...\n", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
        return;
    } else {
        terminal_write_color("Unknown command: ", VGA_COLOR_RED, VGA_COLOR_BLACK);
        terminal_writestring(command);
        terminal_writestring("\nType 'help' for available commands\n");
    }
    terminal_writestring("\n");
}

// Show help information
void show_help(void) {
    terminal_write_color("=== AVAILABLE COMMANDS ===\n", VGA_COLOR_CYAN, VGA_COLOR_BLACK);
    terminal_writestring("help     - Show this help message\n");
    terminal_writestring("sysinfo  - Display system information\n");
    terminal_writestring("security - Show security system status\n");
    terminal_writestring("game     - Launch the built-in game\n");
    terminal_writestring("clear    - Clear the screen\n");
    terminal_writestring("exit     - Shutdown the system\n");
    terminal_write_color("===========================\n", VGA_COLOR_CYAN, VGA_COLOR_BLACK);
}

// Show system information
void show_system_info(void) {
    terminal_write_color("=== SYSTEM INFORMATION ===\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    terminal_writestring("OS Name: UnderTheBoard OS\n");
    terminal_writestring("Version: 1.0\n");
    terminal_writestring("Architecture: x86_64\n");
    terminal_writestring("Kernel: Custom microkernel\n");
    terminal_writestring("Memory: Basic management active\n");
    terminal_writestring("Security: Multi-layer privilege system\n");
    terminal_writestring("Graphics: VGA text mode (80x25)\n");
    terminal_writestring("Build: Debug development version\n");
    terminal_write_color("===========================\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
}

// Handle security commands
void handle_security_command(const char* args) {
    print_security_info();
    
    terminal_write_color("Security Commands:\n", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
    terminal_writestring("- Core mode: Full system access (current)\n");
    terminal_writestring("- User mode: Restricted access\n");
    terminal_writestring("- Memory protection: Active\n");
    terminal_writestring("- Privilege escalation: Controlled\n");
}