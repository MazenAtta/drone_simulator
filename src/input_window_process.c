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
    mvprintw(1, 5, "Drone Controller Simulation");

    mvaddwstr(3, 5, L"⬉ q  ⬆ w  ⬈ e");
    mvaddwstr(4, 5, L"⬅ a  ■ s  ➡ d");
    mvaddwstr(5, 5, L"⬋ z  ⬇ x  ⬊ c");

    mvprintw(7, 5, "Other Commands:");
    mvprintw(8, 5, "[b] begin  |  [s] Stop  |  [r] Reset  |  [k] Quit");
}

void display_drone_state(Drone *drone) {
    mvprintw(10, 5, "Drone Position: X = %f, Y = %f", drone->x, drone->y);
    mvprintw(11, 5, "Velocity: VX = %f, VY = %f", drone->vx, drone->vy);
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
            //mvaddwstr(3, 5, L"⬉ q  ⬆ w  ⬈ e");
            //mvaddwstr(4, 5, L"⬅ a  ■ s  ➡ d");
            //mvaddwstr(5, 5, L"⬋ z  ⬇ x  ⬊ c");
            //mvprintw(8, 5, "[b] begin  |  [s] Stop  |  [r] Reset  |  [k] Quit");

        case 'q': mvprintw(3, 5, "⬉ q"); break;
        case 'w': mvprintw(3, 10, "⬆ w"); break;
        case 'e': mvprintw(3, 15, "⬈ e"); break;
        case 'a': mvprintw(4, 5, "⬅ a"); break;
        case 's': mvprintw(4, 10, "■ s"); mvprintw(8, 20, "s");  break;
        case 'd': mvprintw(4, 15, "➡ d"); break;
        case 'z': mvprintw(5, 5, "⬋ z"); break;
        case 'x': mvprintw(5, 10, "⬇ x"); break;
        case 'c': mvprintw(5, 15, "⬊ c"); break;
        case 'b': mvprintw(8, 6, "b"); break;
        case 'r': mvprintw(8, 33, "r"); break;
        case 'k': mvprintw(8, 47, "k"); break;
    }
    attroff(COLOR_PAIR(1));
    refresh();
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

    display_controls();
    Drone drone = {10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
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