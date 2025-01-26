#ifndef NCURSES_HANDLER_H
#define NCURSES_HANDLER_H

#include <ncurses.h>
#include <string.h>
#include <physics_handler.h>


#define DRONE_SYMBOL '+'
#define FORCE_UP -1
#define FORCE_DOWN 1
#define FORCE_LEFT -1
#define FORCE_RIGHT 1

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

typedef struct {
    float drone_prev_x, drone_prev_y;
    float obstacle_prev_x[MAX_OBSTACLES], obstacle_prev_y[MAX_OBSTACLES];
    float target_prev_x[MAX_TARGETS], target_prev_y[MAX_TARGETS];
} GamePrev;



#endif

