#include "output_window_handler.h"

int main() {
    // Define paths for FIFO and log files
    const char *output_ask = "/tmp/output_ask";
    const char *output_receive = "/tmp/output_receive";
    const char *log_folder = "log";
    const char *log_file = "log/output_window_log.txt";
    const char *config_file = "../config/config.yaml";

    // Create log folder with full permissions
    mkdir(log_folder, 0777);

    // Open FIFOs for communication
    int fd_receive = open(output_receive, O_WRONLY | O_NONBLOCK);
    int fd_ask = open(output_ask, O_RDONLY | O_NONBLOCK);

    // Check if FIFOs are opened successfully
    if (fd_receive == -1 || fd_ask == -1) {
        perror("Failed to open FIFO");
        return 1;
    }

    // Initialize ncurses for graphical output
    init_ncurses();

    // Initialize game entities
    Drone drone;
    Obstacle obstacles;
    Target targets;
    Game game = {0};
    GamePrev game_prev;
    init_drone(&drone);

    // Clear screen and draw initial state
    clear();
    draw_drone(&drone);
    draw_border();
    refresh();

    // Initialize timing variables
    struct timespec last_time = {0};
    time_t last_log_time = 0;

    while (1) {
        // Load configuration file
        load_config(config_file);

        // Read game state from FIFO
        read(fd_ask, &game, sizeof(Game));

        // Update obstacles if game has started or updated
        if (game.game_start == 0 || game.game_update == 1) {
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                obstacles.x[i] = game.Obstacle_x[i];
                obstacles.y[i] = game.Obstacle_y[i];
            }
        }

        // Update targets and score if game has started or updated
        if (game.game_start == 0 || game.game_update == 2) {
            for (int i = 0; i < MAX_TARGETS; i++) {
                targets.x[i] = game.Target_x[i];
                targets.y[i] = game.Target_y[i];
                targets.id[i] = game.target_id[i];
                drone.score = game.score;
            }
        }

        // If game is running
        if (game.game_start == 1) {
            // Update drone state based on command
            update_drone_state(&drone, game.command);
            game.command = 0;

            // Get current time and calculate elapsed time
            struct timespec current_time;
            clock_gettime(CLOCK_MONOTONIC, &current_time);
            double elapsed_time = (current_time.tv_sec - last_time.tv_sec) + 
                                  (current_time.tv_nsec - last_time.tv_nsec) / 1e9;

            // Update game state if enough time has passed
            if (elapsed_time >= TIME_STEP) {
                calculate_total_forces(&drone, &obstacles, &targets);
                update_drone(&drone);
                clear_screen(&drone, &obstacles, &targets, &game_prev);
                score(&drone, &targets);
                draw_obstacles(&obstacles);
                draw_targets(&targets);
                draw_drone(&drone);
                draw_border();
                refresh();
                last_time = current_time;
            }
            // Send updated drone state to FIFO
            write(fd_receive, &drone, sizeof(Drone));
        } else {
            // If game is not running, just redraw the screen
            clear_screen(&drone, &obstacles, &targets, &game_prev);
            draw_obstacles(&obstacles);
            draw_targets(&targets);
            draw_drone(&drone);
            draw_border();
            refresh();
        }

        // Log execution every second
        time_t current_time_2 = time(NULL);
        if (current_time_2 - last_log_time >= 1) {
            log_execution(log_file);
            last_log_time = current_time_2;
        }
    }

    // End ncurses mode and close FIFOs
    endwin();
    close(fd_receive);
    close(fd_ask);
    return 0;
}