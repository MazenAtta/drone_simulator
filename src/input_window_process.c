#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include "obstacle_target_handler.h"

typedef struct {
    float x, y;           // Position
    float vx, vy;         // Velocity
    float ax, ay;         // Acceleration
    float prev_x, prev_y;  // For Euler’s method
    float command_force_x, command_force_y;  // Command force components
    float prev_total_command_force_x, prev_total_command_force_y;
    float force_x,force_y;
    float score;
} Drone;

typedef struct {
    char button;
    time_t highlight_end;
} HighlightState;

typedef struct {
    int command;
    int Obstacle_x[MAX_OBSTACLES], Obstacle_y[MAX_OBSTACLES];
    int Target_x[MAX_TARGETS], Target_y[MAX_TARGETS], target_id[MAX_TARGETS];
    int game_pause;
    int game_start;
    int game_over;
    int game_reset;
    int score;
    int level;
} Game;

void display_controls() {
    mvprintw(2, 6, "Drone Controller");

    mvaddwstr(4, 26, L"⬉ q  ⬆ w  ⬈ e");
    mvaddwstr(5, 26, L"⬅ a  ■ s  ➡ d");
    mvaddwstr(6, 26, L"⬋ z  ⬇ x  ⬊ c");

    attron(A_BOLD);
    mvprintw(8, 6, "Other Commands:");
    attroff(A_BOLD);
    mvprintw(9, 6, "[b] Begin  |  [s] Stop/Start  |  [r] Reset  |  [k] Quit");
}

void display_drone_state(Drone *drone) {
    attron(A_BOLD);
    mvprintw(13, 6, "Drone Position: ");
    attroff(A_BOLD);
    printw("X = %f, Y = %f", drone->x, drone->y);

    attron(A_BOLD);
    mvprintw(14, 6, "Drone Velocity: ");
    attroff(A_BOLD);
    printw("VX = %f, VY = %f", drone->vx, drone->vy);

    attron(A_BOLD);
    mvprintw(15, 6, "Score: ");
    attroff(A_BOLD);
    printw("%f", drone->score);

    refresh();
}

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void highlight_button(HighlightState *state) {
    if (state->highlight_end == 0) return; // No button to highlight

    if (time(NULL) >= state->highlight_end) {
        // Restore the original button color after the highlight duration has passed
        display_controls();
        state->highlight_end = 0;
        refresh();
        return;
    }

    // Highlight the button by changing its color
    attron(COLOR_PAIR(1));
    switch (state->button) {
        case 'q': mvprintw(4, 26, "⬉ q"); break;
        case 'w': mvprintw(4, 31, "⬆ w"); break;
        case 'e': mvprintw(4, 36, "⬈ e"); break;
        case 'a': mvprintw(5, 26, "⬅ a"); break;
        case 's': mvprintw(5, 31, "■ s"); mvprintw(9, 21, "s"); break;
        case 'd': mvprintw(5, 36, "➡ d"); break;
        case 'z': mvprintw(6, 26, "⬋ z"); break;
        case 'x': mvprintw(6, 31, "⬇ x"); break;
        case 'c': mvprintw(6, 36, "⬊ c"); break;
        case 'b': mvprintw(9, 7, "b"); break;
        case 'r': mvprintw(9, 34, "r"); break;
        case 'k': mvprintw(9, 48, "k"); break;
    }
    attroff(COLOR_PAIR(1));
    refresh();
}

void input_display() {
    // Get terminal size
    int rows = 10, cols = 65;

    // Draw the top and bottom borders
    mvhline(1, 1, ACS_HLINE, cols);        // Top border
    mvhline(rows, 1, ACS_HLINE, cols); // Bottom border

    // Draw the left and right borders
    mvvline(1, 1, ACS_VLINE, rows);        // Left border
    mvvline(1, cols, ACS_VLINE, rows); // Right border

    // Draw the corners
    attron(A_BOLD | A_ITALIC);
    mvprintw(1, 3, "Input Controller");
    attroff(A_BOLD | A_ITALIC);
    mvaddch(1, 1, ACS_ULCORNER);             // Top-left corner
    mvaddch(1, cols, ACS_URCORNER);      // Top-right corner
    mvaddch(rows, 1, ACS_LLCORNER);      // Bottom-left corner
    mvaddch(rows, cols, ACS_LRCORNER); // Bottom-right corner
}

void dynamic_display() {
    // Get terminal size
    int rows = 18, cols = 65;

    // Draw the top and bottom borders
    mvhline(11, 1, ACS_HLINE, cols);        // Top border
    mvhline(rows, 1, ACS_HLINE, cols); // Bottom border

    // Draw the left and right borders
    mvvline(11, 1, ACS_VLINE, rows - 11);        // Left border
    mvvline(11, cols, ACS_VLINE, rows - 11); // Right border

    // Draw the corners
    attron(A_BOLD | A_ITALIC);
    mvprintw(11, 3, "Dynamic Controller");
    attroff(A_BOLD | A_ITALIC);
    mvaddch(11, 1, ACS_ULCORNER);             // Top-left corner
    mvaddch(11, cols, ACS_URCORNER);      // Top-right corner
    mvaddch(rows, 1, ACS_LLCORNER);      // Bottom-left corner
    mvaddch(rows, cols, ACS_LRCORNER); // Bottom-right corner
}

int main() {
    setlocale(LC_ALL, ""); // Set locale to the user's default locale

    const char *input_ask = "/tmp/input_ask";
    const char *input_receive = "/tmp/input_receive";

    int fd_receive = open(input_receive, O_WRONLY);
    int fd_ask = open(input_ask, O_RDONLY | O_NONBLOCK);
    Game game;
    initscr();
    start_color(); // Start color functionality
    init_pair(1, COLOR_RED, COLOR_BLACK); // Initialize color pair 1 with red text on black background
    noecho();
    cbreak();
    nodelay(stdscr, TRUE); // Make getch() non-blocking

    input_display();
    display_controls();
    dynamic_display();
    Drone drone;
    display_drone_state(&drone);

    int ch;
    HighlightState highlight_state = {0, 0}; // Initialize the highlight state

    while (1) {
        ch = getch();
        if (ch != ERR) { // If a key was pressed
            switch (ch) {
                case 'b': case 'q': case 'w': case 'e': case 'a': case 's': case 'd': case 'z': case 'x': case 'c' : case 'p': case 'r': case 'k':
                    if(ch != 'b') game.command = ch;
                    else if(ch == 'b') game.game_start = 1;
                    write(fd_receive, &game, sizeof(Game));
                    highlight_state.button = ch;
                    highlight_state.highlight_end = time(NULL) + 1; // Highlight for 1 second
                    break;
                default:
                    refresh();
            }
        }

        ssize_t bytes_read = read(fd_ask, &drone, sizeof(Drone));
        if (bytes_read > 0) {
            display_drone_state(&drone);
        }

        highlight_button(&highlight_state);
    }

    endwin();
    close(fd_receive);
    return 0;
}
