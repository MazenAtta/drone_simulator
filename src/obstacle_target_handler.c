#include <stdlib.h>
#include "obstacle_target_handler.h"

void init_obstacles(Obstacle obstacles[]) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].x = -1; // Off-screen by default
        obstacles[i].y = -1;
    }
}

void init_targets(Target targets[]) {
    for (int i = 0; i < MAX_TARGETS; i++) {
        targets[i].x = -1;
        targets[i].y = -1;
        targets[i].id = i + 1;
    }
}

void generate_obstacles(Obstacle obstacles[]) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (rand() % 10 < 7) { // 70% chance of generating
            obstacles[i].x = rand() % COLS;
            obstacles[i].y = rand() % LINES;
        }
    }
}

void generate_targets(Target targets[]) {
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (rand() % 10 < 5) { // 50% chance
            targets[i].x = rand() % COLS;
            targets[i].y = rand() % LINES;
        }
    }
}

void draw_obstacles(Obstacle obstacles[]) {
    attron(COLOR_PAIR(2)); // Orange color for obstacles
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].x != -1 && obstacles[i].y != -1) {
            mvaddch(obstacles[i].y, obstacles[i].x, 'O');
        }
    }
    attroff(COLOR_PAIR(2));
}

void draw_targets(Target targets[]) {
    attron(COLOR_PAIR(3)); // Green color for targets
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets[i].x != -1 && targets[i].y != -1) {
            mvaddch(targets[i].y, targets[i].x, '1' + i);
        }
    }
    attroff(COLOR_PAIR(3));
}
