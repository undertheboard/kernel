#include "../include/kernel.h"

// Game state
typedef struct {
    int player_x;
    int player_y;
    int score;
    int level;
    char field[10][20];
} game_state_t;

static game_state_t game;

// Game function prototypes
void init_game(void);
void draw_game(void);
void game_loop(void);
void move_player(char direction);
void generate_level(void);

// Launch the game
void launch_game(void) {
    terminal_clear();
    terminal_write_color("=== UNDERTHEBOARD ADVENTURE ===\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    terminal_writestring("A simple text-based adventure game\n\n");
    
    terminal_writestring("Game Story:\n");
    terminal_writestring("You are an explorer in the UnderTheBoard realm.\n");
    terminal_writestring("Navigate through the maze and collect treasures!\n\n");
    
    terminal_writestring("Controls (simulated for demo):\n");
    terminal_writestring("W - Move Up\n");
    terminal_writestring("S - Move Down\n");
    terminal_writestring("A - Move Left\n");
    terminal_writestring("D - Move Right\n");
    terminal_writestring("Q - Quit Game\n\n");
    
    // Initialize and run game
    init_game();
    draw_game();
    game_loop();
    
    terminal_writestring("\nReturning to console...\n");
}

// Initialize game state
void init_game(void) {
    game.player_x = 1;
    game.player_y = 1;
    game.score = 0;
    game.level = 1;
    generate_level();
}

// Generate a simple level
void generate_level(void) {
    // Initialize field with walls
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 20; x++) {
            if (y == 0 || y == 9 || x == 0 || x == 19) {
                game.field[y][x] = '#'; // Wall
            } else {
                game.field[y][x] = '.'; // Empty space
            }
        }
    }
    
    // Add some treasures
    game.field[3][5] = '$';
    game.field[7][15] = '$';
    game.field[5][10] = '$';
    game.field[2][8] = '$';
    
    // Add some obstacles
    game.field[4][4] = '#';
    game.field[6][12] = '#';
    game.field[8][7] = '#';
    
    // Player starting position
    game.field[game.player_y][game.player_x] = '@';
}

// Draw the game field
void draw_game(void) {
    terminal_clear();
    terminal_write_color("=== UNDERTHEBOARD ADVENTURE ===\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    
    // Display score and level
    terminal_write_color("Score: ", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
    // Simple integer to string conversion for score
    if (game.score == 0) {
        terminal_writestring("0");
    } else {
        terminal_writestring("100"); // Simplified for demo
    }
    
    terminal_write_color("  Level: ", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
    terminal_writestring("1\n\n");
    
    // Draw the game field
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 20; x++) {
            char c = game.field[y][x];
            
            if (c == '@') {
                terminal_write_color("@", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
            } else if (c == '$') {
                terminal_write_color("$", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
            } else if (c == '#') {
                terminal_write_color("#", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
            } else {
                terminal_write_color(".", VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
            }
        }
        terminal_writestring("\n");
    }
    
    terminal_writestring("\nLegend:\n");
    terminal_write_color("@ ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("- Player\n");
    terminal_write_color("$ ", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
    terminal_writestring("- Treasure\n");
    terminal_write_color("# ", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    terminal_writestring("- Wall/Obstacle\n");
    terminal_write_color(". ", VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
    terminal_writestring("- Empty space\n");
}

// Move player
void move_player(char direction) {
    int new_x = game.player_x;
    int new_y = game.player_y;
    
    switch (direction) {
        case 'w': case 'W': new_y--; break;
        case 's': case 'S': new_y++; break;
        case 'a': case 'A': new_x--; break;
        case 'd': case 'D': new_x++; break;
    }
    
    // Check bounds and collisions
    if (new_x >= 0 && new_x < 20 && new_y >= 0 && new_y < 10) {
        char target = game.field[new_y][new_x];
        
        if (target != '#') {
            // Clear old position
            game.field[game.player_y][game.player_x] = '.';
            
            // Check for treasure
            if (target == '$') {
                game.score += 100;
                terminal_write_color("\nTreasure collected! +100 points\n", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
            }
            
            // Move player
            game.player_x = new_x;
            game.player_y = new_y;
            game.field[new_y][new_x] = '@';
        }
    }
}

// Game loop (simplified for demo)
void game_loop(void) {
    // Simulate some moves for the demo
    char demo_moves[] = "ddddssssaaaaawwwwq";
    int move_index = 0;
    
    while (demo_moves[move_index] != 'q' && move_index < sizeof(demo_moves) - 1) {
        char move = demo_moves[move_index++];
        
        if (move == 'q') {
            break;
        }
        
        move_player(move);
        draw_game();
        
        // Add some delay simulation
        terminal_writestring("\n[Simulated move: ");
        terminal_putchar(move);
        terminal_writestring("]\n");
        
        // Simple delay (in a real OS, we'd use proper timing)
        for (volatile int i = 0; i < 1000000; i++);
    }
    
    terminal_write_color("\nGame Over!\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
    terminal_write_color("Final Score: ", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
    if (game.score > 0) {
        terminal_writestring("200");
    } else {
        terminal_writestring("0");
    }
    terminal_writestring("\n");
    terminal_writestring("Thanks for playing UnderTheBoard Adventure!\n");
}