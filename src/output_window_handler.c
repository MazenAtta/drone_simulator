#include "output_window_handler.h"

// Initialize configuration variables with default values
float DRONE_MASS = 1.0;
float DRONE_FRICTION = 1.0;
float REPULSIVE_CONSTANT = 10.0;
float PERCEPTION_RADIUS = 5.0;
float TIME_STEP = 0.01;
int OBSTACLES_HIT = 0;

// Load configuration from a YAML file
void load_config(const char *config_file) {
    FILE *file = fopen(config_file, "r");
    if (!file) {
        perror("Failed to open configuration file");
        return;
    }

    yaml_parser_t parser;
    yaml_document_t document;
    yaml_node_t *node, *key_node, *value_node;

    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize YAML parser\n");
        fclose(file);
        return;
    }

    yaml_parser_set_input_file(&parser, file);

    if (!yaml_parser_load(&parser, &document)) {
        fprintf(stderr, "Failed to load YAML document\n");
        yaml_parser_delete(&parser);
        fclose(file);
        return;
    }

    node = yaml_document_get_root_node(&document);
    if (node && node->type == YAML_MAPPING_NODE) {
        for (yaml_node_pair_t *pair = node->data.mapping.pairs.start;
             pair < node->data.mapping.pairs.top; pair++) {
            key_node = yaml_document_get_node(&document, pair->key);
            value_node = yaml_document_get_node(&document, pair->value);
            if (key_node && value_node && key_node->type == YAML_SCALAR_NODE && value_node->type == YAML_SCALAR_NODE) {
                const char *key = (const char *)key_node->data.scalar.value;
                const char *value = (const char *)value_node->data.scalar.value;
                char *endptr;
                float val = strtof(value, &endptr);

                if (*endptr != '\0') {
                    mvprintw(30 - 1, 0, "Invalid value for %s: %s. Reverting to default.", key, value);
                    continue;
                }

                if (strcmp(key, "DRONE_MASS") == 0) {
                    if (val > 0) DRONE_MASS = val;
                } else if (strcmp(key, "DRONE_FRICTION") == 0) {
                    if (val >= 0) DRONE_FRICTION = val;
                } else if (strcmp(key, "REPULSIVE_CONSTANT") == 0) {
                    if (val >= 0) REPULSIVE_CONSTANT = val;
                } else if (strcmp(key, "PERCEPTION_RADIUS") == 0) {
                    if (val > 0) PERCEPTION_RADIUS = val;
                } else if (strcmp(key, "TIME_STEP") == 0) {
                    if (val >= 0.01 && val <= 0.1) TIME_STEP = val;
                }
            }
        }
    }

    yaml_document_delete(&document);
    yaml_parser_delete(&parser);
    fclose(file);
}

// Initialize ncurses library for terminal graphics
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

// Log execution details to a file
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

// Draw the drone on the screen
void draw_drone(Drone *drone) {
    mvaddch((int)drone->y, (int)drone->x, DRONE_SYMBOL | COLOR_PAIR(1));
}

// Draw obstacles on the screen
void draw_obstacles(Obstacle *obstacles) {
    attron(COLOR_PAIR(2)); // Orange color for obstacles
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles->x[i] != -1 && obstacles->y[i] != -1) {
            mvaddch(obstacles->y[i], obstacles->x[i], 'O');
        }
    }
    attroff(COLOR_PAIR(2));
}

// Draw targets on the screen
void draw_targets(Target *targets) {
    attron(COLOR_PAIR(3)); // Green color for targets
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets->x[i] != -1 && targets->y[i] != -1) {
            mvaddch(targets->y[i], targets->x[i], '1' + i);
        }
    }
    attroff(COLOR_PAIR(3));
}

// Draw the border of the simulation area
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

// Initialize the drone with default values
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

// Update the drone's state based on user commands
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
    if (drone->command_force_x > 5) drone->command_force_x = 5;
    if (drone->command_force_x < -5) drone->command_force_x = -5;
    if (drone->command_force_y > 5) drone->command_force_y = 5;
    if (drone->command_force_y < -5) drone->command_force_y = -5;
}

// Calculate the total forces acting on the drone
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
            if (distance <= 1) {
                OBSTACLES_HIT++;
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
        if (distance <= 1) {
            OBSTACLES_HIT++;
        }
    }

    if (cols - drone->x < PERCEPTION_RADIUS) {
        float distance = cols - drone->x;
        if (distance > 0) {
            total_force_x -= border_repulsion_factor * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
        }
        if (distance <= 1) {
            OBSTACLES_HIT++;
        }
    }

    if (drone->y < PERCEPTION_RADIUS) {
        float distance = drone->y;
        if (distance > 0) {
            total_force_y += border_repulsion_factor * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
        }
        if (distance <= 1) {
            OBSTACLES_HIT++;
        }
    }

    if (rows - drone->y < PERCEPTION_RADIUS) {
        float distance = rows - drone->y;
        if (distance > 0) {
            total_force_y -= border_repulsion_factor * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
        }
        if (distance <= 1) {
            OBSTACLES_HIT++;
        }
    }

    drone->force_x = total_force_x;
    drone->force_y = total_force_y;
}

// Update the drone's position and velocity
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

// Clear the screen and update previous positions
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

// Calculate the score based on targets hit and obstacles encountered
void score(Drone *drone, Target *targets) {
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets->x[i] != -1 && targets->y[i] != -1 && drone->score == (i)) {
            float dx = drone->x - targets->x[i];
            float dy = drone->y - targets->y[i];
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= .9) {
                mvaddch(targets->y[i], targets->x[i], ' ');
                targets->x[i] = -1;
                targets->y[i] = -1;
                drone->score += 1;

                // Calculate actual score
                float obstacle_penalty = 0.1 * OBSTACLES_HIT;

                drone->actual_score += 10; // Base score for hitting a target
                drone->actual_score -= obstacle_penalty; // Penalty for obstacles
                OBSTACLES_HIT = 0;
            }
        }
    }
}