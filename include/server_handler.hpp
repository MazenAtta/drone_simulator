#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <poll.h>

#define MAX_OBSTACLES 10
#define MAX_TARGETS 5
#define NUM_PIPES 6

// Struct to represent drone dynamics
typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    float prev_x, prev_y;
    float command_force_x, command_force_y;
    float prev_total_command_force_x, prev_total_command_force_y;
    float force_x, force_y;
    float score;
    float actual_score;
} Drone;

typedef struct {
    int x[MAX_OBSTACLES], y[MAX_OBSTACLES];
} Obstacle;

typedef struct {
    int x[MAX_TARGETS], y[MAX_TARGETS];
    int id[MAX_TARGETS];
} Target;

typedef struct {
    int command;
    int Obstacle_x[MAX_OBSTACLES], Obstacle_y[MAX_OBSTACLES];
    int Target_x[MAX_TARGETS], Target_y[MAX_TARGETS], target_id[MAX_TARGETS];
    int game_update;
    int game_start;
    int game_over;
    int game_reset;
    int score;
    int level;
} Game;

void error_exit(const char *msg);
void log_execution(const char *log_file);
void update_game_state(Game *game, Obstacle *obstacles, Target *targets);
int open_pipe(const char *pipe_name, int flags);

#endif // SERVER_HANDLER_H