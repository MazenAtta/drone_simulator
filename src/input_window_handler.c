#include "input_window_handler.h"

// Display the control instructions on the screen
void display_controls() {
    mvprintw(2, 6, "Drone Controller");

    mvaddwstr(4, 26, L"⬉ q  ⬆ w  ⬈ e");
    mvaddwstr(5, 26, L"⬅ a  ■ s  ➡ d");
    mvaddwstr(6, 26, L"⬋ z  ⬇ x  ⬊ c");

    attron(A_BOLD);
    mvprintw(8, 6, "Other Commands:");
    attroff(A_BOLD);
    mvprintw(9, 6, "[b] Begin  |  [p] Stop/Start  |  [r] Reset  |  [k] Quit");
}

// Display the current state of the drone
void display_drone_state(Drone *drone) {
    attron(A_BOLD);
    mvprintw(13, 6, "Drone Position: ");
    attroff(A_BOLD);
    printw("X = %f, Y = %f", drone->x, drone->y);

    attron(A_BOLD);
    mvprintw(14, 6, "Drone Command Forces: ");
    attroff(A_BOLD);
    printw("FCX = %f, FCY = %f", drone->command_force_x, drone->command_force_y);

    attron(A_BOLD);
    mvprintw(15, 6, "Drone Velocity: ");
    attroff(A_BOLD);
    printw("VX = %f, VY = %f", drone->vx, drone->vy);

    attron(A_BOLD);
    mvprintw(16, 6, "Score: ");
    attroff(A_BOLD);
    printw("%f", drone->actual_score);

    refresh();
}

// Print an error message and exit the program
void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Log the execution details to a file
void log_execution(const char *log_file) {
    FILE *log_fp = fopen(log_file, "a");
    if (log_fp == NULL) {
        error_exit("Failed to open log file");
    }

    time_t now = time(NULL);
    if (now == (time_t)-1) {
        error_exit("Failed to get current time");
    }

    fprintf(log_fp, "PID: %d, Time: %ld\n", getpid(), now);
    fclose(log_fp);
}

// Highlight a specific button on the screen
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
        case 's': mvprintw(5, 31, "■ s"); break;
        case 'd': mvprintw(5, 36, "➡ d"); break;
        case 'z': mvprintw(6, 26, "⬋ z"); break;
        case 'x': mvprintw(6, 31, "⬇ x"); break;
        case 'c': mvprintw(6, 36, "⬊ c"); break;
        case 'b': mvprintw(9, 7, "b"); break;
        case 'p': mvprintw(9, 21, "p"); break;
        case 'r': mvprintw(9, 40, "r"); break;
        case 'k': mvprintw(9, 54, "k"); break;
    }
    attroff(COLOR_PAIR(1));
    refresh();
}

// Display the input controller window
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

// Display the dynamic controller window
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