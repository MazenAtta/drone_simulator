#include "output_window_handler.h"

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

void log_execution(const char *log_file) {
    FILE *log_fp = fopen(log_file, "a");
    if (log_fp == NULL) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }

    time_t now = time(NULL);
    if (now == (time_t)-1) {
        perror("Failed to get current time");
        exit(EXIT_FAILURE);
    }

    fprintf(log_fp, "PID: %d, Time: %ld\n", getpid(), now);
    fclose(log_fp);
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
    drone->score = 0;
}

void update_drone_state(Drone *drone, char command) {
    switch (command) {
        case 'q': drone->command_force_y += cos(45) * FORCE_UP; drone->command_force_x +=  cos(45) * FORCE_LEFT; break;
        case 'w': drone->command_force_y += FORCE_UP; break;
        case 'e': drone->command_force_y += cos(45) * FORCE_UP; drone->command_force_x += cos(45) * FORCE_RIGHT; break;
        case 'a': drone->command_force_x += FORCE_LEFT; break;
        case 's': drone->command_force_y = 0; drone->command_force_x = 0; break;
        case 'd': drone->command_force_x += FORCE_RIGHT; break;
        case 'z': drone->command_force_y += cos(45) * FORCE_DOWN; drone->command_force_x += cos(45) * FORCE_LEFT; break;
        case 'x': drone->command_force_y += FORCE_DOWN; break;
        case 'c': drone->command_force_y += cos(45) * FORCE_DOWN; drone->command_force_x += cos(45) * FORCE_RIGHT; break;
        default: break;
    }
}

void calculate_total_forces(Drone *drone, Obstacle *obstacles, Target *targets) {
    float total_force_x = drone->command_force_x;
    float total_force_y = drone->command_force_y;

    float drag_force_x = -DRONE_FRICTION * drone->vx;
    float drag_force_y = -DRONE_FRICTION * drone->vy;

    total_force_x += drag_force_x;
    total_force_y += drag_force_y;

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
        else if (obstacles->x[i] == targets->x[i] && obstacles->y[i] == targets->y[i]) {
            obstacles->x[i] = -1;
            obstacles->y[i] = -1;
        }
    }

    int rows = 30, cols = 90;
    float border_repulsion_factor = REPULSIVE_CONSTANT * 10;

    if (drone->x < PERCEPTION_RADIUS) {
        float distance = drone->x;
        if (distance > 0) {
            total_force_x += border_repulsion_factor * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
        }
    }

    if (cols - drone->x < PERCEPTION_RADIUS) {
        float distance = cols - drone->x;
        if (distance > 0) {
            total_force_x -= border_repulsion_factor * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
        }
    }

    if (drone->y < PERCEPTION_RADIUS) {
        float distance = drone->y;
        if (distance > 0) {
            total_force_y += border_repulsion_factor * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
        }
    }

    if (rows - drone->y < PERCEPTION_RADIUS) {
        float distance = rows - drone->y;
        if (distance > 0) {
            total_force_y -= border_repulsion_factor * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
        }
    }

    drone->force_x = total_force_x;
    drone->force_y = total_force_y;
}

void update_drone(Drone *drone) {
    drone->ax = drone->force_x / DRONE_MASS;
    drone->ay = drone->force_y / DRONE_MASS;

    drone->vx = drone->vx + drone->ax * TIME_STEP;
    drone->vy = drone->vy + drone->ay * TIME_STEP;

    float new_x = drone->x + drone->vx * TIME_STEP + 0.5 * drone->ax * TIME_STEP * TIME_STEP;
    float new_y = drone->y + drone->vy * TIME_STEP + 0.5 * drone->ay * TIME_STEP * TIME_STEP;

    if (new_x < drone->x - 0.5 || new_x > drone->x + 0.5) {
        new_x = drone->x;
    }

    if (new_y < drone->y - 0.5 || new_y > drone->y + 0.5) {
        new_y = drone->y;
    }

    drone->prev_x = drone->x;
    drone->prev_y = drone->y;
    drone->x = new_x;
    drone->y = new_y;
}

void clear_screen(Drone *drone, Obstacle *obstacles, Target *targets, GamePrev *game_prev) {
    if ((int)drone->prev_y != (int)drone->y || (int)drone->prev_x != (int)drone->x) {
        mvaddch((int)drone->prev_y, (int)drone->prev_x, ' ');
        game_prev->drone_prev_x = drone->x;
        game_prev->drone_prev_y = drone->y;
    }

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles->x[i] != -1 && obstacles->y[i] != -1) {
            if ((int)game_prev->obstacle_prev_x[i] != obstacles->x[i] || (int)game_prev->obstacle_prev_y[i] != obstacles->y[i]) {
                mvaddch(game_prev->obstacle_prev_y[i], game_prev->obstacle_prev_x[i], ' ');
                game_prev->obstacle_prev_x[i] = obstacles->x[i];
                game_prev->obstacle_prev_y[i] = obstacles->y[i];
            }
        }
    }

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
                targets->x[i] = -1;
                targets->y[i] = -1;
                drone->score += 1;
            }
        }
    }
}