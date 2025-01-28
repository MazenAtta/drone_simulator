#ifndef INPUT_WINDOW_HANDLER_H
#define INPUT_WINDOW_HANDLER_H

#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>

#define MAX_OBSTACLES 10
#define MAX_TARGETS 5

typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    float prev_x, prev_y;
    float command_force_x, command_force_y;
    float prev_total_command_force_x, prev_total_command_force_y;
    float force_x, force_y;
    float score;
} Drone;

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
    char button;
    time_t highlight_end;
} HighlightState;

void display_controls();
void display_drone_state(Drone *drone);
void error_exit(const char *msg);
void log_execution(const char *log_file);
void highlight_button(HighlightState *state);
void input_display();
void dynamic_display();

#endif // INPUT_WINDOW_HANDLER_H