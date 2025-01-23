#include "ncurses_handler.h"
#include "obstacle_target_handler.h"




void init_ncurses() {
    initscr();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);  // Obstacles
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // Targets
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    clear();
    refresh();
}


void draw_drone(Drone *drone, char *status) {
    mvaddch((float)drone->y, (float)drone->x, DRONE_SYMBOL| COLOR_PAIR(1));
}

void close_ncurses() {
    endwin();            // Terminate ncurses mode
}
