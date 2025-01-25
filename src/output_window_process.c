#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include "physics_handler.h"
#include "obstacle_target_handler.h"

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

void init_ncurses() {
    initscr();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);  // Obstacles
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // Targets
    noecho();
    cbreak();
    curs_set(0);
    clear();
    refresh();
}


void draw_drone(Drone *drone) {
    mvaddch((int)drone->y, (int)drone->x, DRONE_SYMBOL | COLOR_PAIR(1));
}

void draw_obstacles(Obstacle *obstacles) {
    attron(COLOR_PAIR(2)); // Orange color for obstacles
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles->x[i] != -1 && obstacles->y[i] != -1) {
            mvaddch(obstacles->y[i], obstacles->x[i], 'O');
        }
    }
    attroff(COLOR_PAIR(2));
}

void draw_targets(Target *targets) {
    attron(COLOR_PAIR(3)); // Green color for targets
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets->x[i] != -1 && targets->y[i] != -1) {
            mvaddch(targets->y[i], targets->x[i], '1' + i);
        }
    }
    attroff(COLOR_PAIR(3));
}

void draw_border() {
    // Get terminal size
    int rows = 30, cols = 90;

    // Draw the top and bottom borders
    mvhline(0, 0, ACS_HLINE, cols);        // Top border
    mvhline(rows - 1, 0, ACS_HLINE, cols); // Bottom border

    // Draw the left and right borders
    mvvline(0, 0, ACS_VLINE, rows);        // Left border
    mvvline(0, cols - 1, ACS_VLINE, rows); // Right border

    // Draw the corners
    attron(A_BOLD | A_ITALIC);
    mvprintw(0, 3, "Dynamics Simulation");
    attroff(A_BOLD | A_ITALIC);
    mvaddch(0, 0, ACS_ULCORNER);             // Top-left corner
    mvaddch(0, cols - 1, ACS_URCORNER);      // Top-right corner
    mvaddch(rows - 1, 0, ACS_LLCORNER);      // Bottom-left corner
    mvaddch(rows - 1, cols - 1, ACS_LRCORNER); // Bottom-right corner
}


// Initialize the drone's parameters
void init_drone(Drone *drone) {
    drone->x = 10.0;
    drone->y = 10.0;
    drone->vx = 0.0;
    drone->vy = 0.0;
    drone->ax = 0.0;
    drone->ay = 0.0;
    drone->prev_x = 10.0;
    drone->prev_y = 10.0;
    drone->command_force_x = 0.0;
    drone->command_force_y = 0.0;
    drone->prev_total_command_force_x = 0.0;
    drone->prev_total_command_force_y = 0.0;
}

void update_drone_state(Drone *drone, char command) {
    // Reset command forces
    drone->command_force_x = 0.0;
    drone->command_force_y = 0.0;
    switch (command) {
        case 'q': drone->command_force_y = cos(45) * FORCE_UP; drone->command_force_x =  cos(45) * FORCE_LEFT; break;
        case 'w': drone->command_force_y = FORCE_UP; break;
        case 'e': drone->command_force_y = cos(45) * FORCE_UP; drone->command_force_x = cos(45) * FORCE_RIGHT; break;
        case 'a': drone->command_force_x = FORCE_LEFT; break;
        case 's': drone->command_force_y = 0; drone->command_force_x = 0; break;
        case 'd': drone->command_force_x = FORCE_RIGHT; break;
        case 'z': drone->command_force_y = cos(45) * FORCE_DOWN; drone->command_force_x = cos(45) * FORCE_LEFT; break;
        case 'x': drone->command_force_y = FORCE_DOWN; break;
        case 'c': drone->command_force_y = cos(45) * FORCE_DOWN; drone->command_force_x = cos(45) * FORCE_RIGHT; break;
        default: break;
    }
}

// Calculate total forces (command, repulsion, attraction, and drag)
void calculate_total_forces(Drone *drone, Obstacle *obstacles, Target *targets) {
    // Start with command forces and accumulate continuous command force
    float total_force_x = drone->command_force_x;
    float total_force_y = drone->command_force_y;

    // Calculate drag forces (viscous friction)
    float drag_force_x = -DRONE_FRICTION * drone->vx;
    float drag_force_y = -DRONE_FRICTION * drone->vy;

    // Add drag forces
    total_force_x += drag_force_x;
    total_force_y += drag_force_y;

    // Add repulsive forces
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles->x[i] != -1 && obstacles->x[i] != targets->x[i] && obstacles->y[i] != -1 && obstacles->y[i] != targets->y[i]) {
            float dx = drone->x - obstacles->x[i];
            float dy = drone->y - obstacles->y[i];
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < PERCEPTION_RADIUS && distance > 0) {
                float repulsion_factor = REPULSIVE_CONSTANT * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
                total_force_x += repulsion_factor * (dx / distance);
                total_force_y += repulsion_factor * (dy / distance);
            }
        }
        else if (obstacles->x[i] == targets->x[i] && obstacles->y[i] == targets->y[i])
        {
            obstacles->x[i] = -1;
            obstacles->y[i] = -1;
        }
        
    }

    // (Optional)
    // Add attractive forces
    /*for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets[i].x != -1) {
            float dx = targets[i].x - drone->x;
            float dy = targets[i].y - drone->y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < PERCEPTION_RADIUS && distance > 0) {  // Avoid zero division
                float attraction_factor = ATTRACTIVE_CONSTANT * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
                total_force_x += attraction_factor * dx / distance;
                total_force_y += attraction_factor * dy / distance;
            }
        }
    }
    */
    /*if (total_force_x < 1) {
        total_force_x = 0;
    } else if (total_force_y < 1) {
        total_force_y = 0;
    }*/

    // Store the total force for use in the update function
    drone->force_x = total_force_x;
    drone->force_y = total_force_y;
}

// Update the drone's dynamics using Newton's laws
void update_drone(Drone *drone) {
    // Calculate acceleration from the total force
    drone->ax = drone->force_x / DRONE_MASS;
    drone->ay = drone->force_y / DRONE_MASS;

    // Update velocity using v = v + a * TIME_STEP
    drone->vx = drone->vx + drone->ax * TIME_STEP;
    drone->vy = drone->vy + drone->ay * TIME_STEP;

    // Update position using x = x + v * TIME_STEP + 0.5 * a * TIME_STEP^2
    float new_x = drone->x + drone->vx * TIME_STEP + 0.5 * drone->ax * TIME_STEP * TIME_STEP;
    float new_y = drone->y + drone->vy * TIME_STEP + 0.5 * drone->ay * TIME_STEP * TIME_STEP;

    // Ensure new_x is within ±0.5 of the old x
    if (new_x < drone->x - 0.5 || new_x > drone->x + 0.5) {
        new_x = drone->x;
    }

    // Ensure new_y is within ±0.5 of the old y
    if (new_y < drone->y - 0.5 || new_y > drone->y + 0.5) {
        new_y = drone->y;
    }

    // Update the drone's state
    drone->prev_x = drone->x;
    drone->prev_y = drone->y;
    drone->x = new_x;
    drone->y = new_y;
}

void clear_screen(Drone *drone, Obstacle *obstacles, Target *targets, GamePrev *game_prev) 
{
    // Clear previous drone position if different
    if ((int)drone->prev_y != (int)drone->y || (int)drone->prev_x != (int)drone->x) {
        mvaddch((int)drone->prev_y, (int)drone->prev_x, ' ');
        game_prev->drone_prev_x = drone->x;
        game_prev->drone_prev_y = drone->y;
    }

    // Clear previous obstacles positions if different
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles->x[i] != -1 && obstacles->y[i] != -1) {
            if ((int)game_prev->obstacle_prev_x[i] != obstacles->x[i] || (int)game_prev->obstacle_prev_y[i] != obstacles->y[i]) {
                mvaddch(game_prev->obstacle_prev_y[i], game_prev->obstacle_prev_x[i], ' ');
                game_prev->obstacle_prev_x[i] = obstacles->x[i];
                game_prev->obstacle_prev_y[i] = obstacles->y[i];
            }
        }
    }

    // Clear previous targets positions if different
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets->x[i] != -1 && targets->y[i] != -1) {
            if ((int)game_prev->target_prev_x[i] != targets->x[i] || (int)game_prev->target_prev_y[i] != targets->y[i]) {
                mvaddch(game_prev->target_prev_y[i], game_prev->target_prev_x[i], ' ');
                game_prev->target_prev_x[i] = targets->x[i];
                game_prev->target_prev_y[i] = targets->y[i];
            }
        }
    }
}

void score(Drone *drone, Target *targets) {
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets->x[i] != -1 && targets->y[i] != -1) {
            float dx = drone->x - targets->x[i];
            float dy = drone->y - targets->y[i];
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= 1) {
                // Clear the target
                targets->x[i] = -1;
                targets->y[i] = -1;
                // Increase the score
                drone->score += 1;
            }
        }
    }
}

int main() {
    const char *output_ask = "/tmp/output_ask";
    const char *output_receive = "/tmp/output_receive";

    int fd_receive = open(output_receive, O_WRONLY | O_NONBLOCK);
    int fd_ask = open(output_ask, O_RDONLY | O_NONBLOCK);

    init_ncurses();

    Drone drone;
    Obstacle obstacles;
    Target targets;
    Game game;
    GamePrev game_prev;
    init_drone(&drone);
    clear();
    draw_drone(&drone);
    draw_border();
    refresh();

    while (1) 
    {
        
        if (game.game_start == 0) {
            read(fd_ask, &game, sizeof(Game));
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                obstacles.x[i] = game.Obstacle_x[i];
                obstacles.y[i] = game.Obstacle_y[i];
            }
            for (int i = 0; i < MAX_TARGETS; i++) {
                targets.x[i]= game.Target_x[i];
                targets.y[i]= game.Target_y[i];
                targets.id[i] = game.target_id[i];
            }
            clear_screen(&drone, &obstacles, &targets, &game_prev);
            draw_obstacles(&obstacles);
            draw_targets(&targets);
            draw_drone(&drone);
            draw_border();
            refresh();
        } 
        if (game.game_start == 1) {
            ssize_t bytes_read = read(fd_ask, &game, sizeof(Game));
            if (bytes_read > 0) {
                update_drone_state(&drone, game.command);
                calculate_total_forces(&drone, &obstacles, &targets);
                update_drone(&drone);
                clear_screen(&drone, &obstacles, &targets, &game_prev);
                score(&drone, &targets);
                draw_obstacles(&obstacles);
                draw_targets(&targets);
                draw_drone(&drone);
                draw_border();
                refresh();
                write(fd_receive, &drone, sizeof(Drone));
            }
        }
    }

    endwin();
    close(fd_receive);
    close(fd_ask);
    return 0;
}