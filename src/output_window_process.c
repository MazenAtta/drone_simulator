#include "output_window_handler.h"

int main() {
    const char *output_ask = "/tmp/output_ask";
    const char *output_receive = "/tmp/output_receive";
    const char *log_folder = "log";
    const char *log_file = "log/output_window_log.txt";
    const char *config_file = "config/config.yaml";
    mkdir(log_folder, 0777);

    int fd_receive = open(output_receive, O_WRONLY | O_NONBLOCK);
    int fd_ask = open(output_ask, O_RDONLY | O_NONBLOCK);

    if (fd_receive == -1 || fd_ask == -1) {
        perror("Failed to open FIFO");
        return 1;
    }

    init_ncurses();

    Drone drone;
    Obstacle obstacles;
    Target targets;
    Game game = {0};
    GamePrev game_prev;
    init_drone(&drone);
    clear();
    draw_drone(&drone);
    draw_border();
    refresh();

    struct timespec last_time = {0};
    time_t last_log_time = 0;

    while (1) {
        load_config(config_file);
        read(fd_ask, &game, sizeof(Game));

        if (game.game_start == 0 || game.game_update == 1) {
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                obstacles.x[i] = game.Obstacle_x[i];
                obstacles.y[i] = game.Obstacle_y[i];
            }
        }

        if (game.game_start == 0 || game.game_update == 2) {
            for (int i = 0; i < MAX_TARGETS; i++) {
                targets.x[i] = game.Target_x[i];
                targets.y[i] = game.Target_y[i];
                targets.id[i] = game.target_id[i];
            }
        }

        if (game.game_start == 1) {
            update_drone_state(&drone, game.command);
            game.command = 0;

            struct timespec current_time;
            clock_gettime(CLOCK_MONOTONIC, &current_time);
            double elapsed_time = (current_time.tv_sec - last_time.tv_sec) + 
                                  (current_time.tv_nsec - last_time.tv_nsec) / 1e9;

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
            write(fd_receive, &drone, sizeof(Drone));
        } else {
            clear_screen(&drone, &obstacles, &targets, &game_prev);
            draw_obstacles(&obstacles);
            draw_targets(&targets);
            draw_drone(&drone);
            draw_border();
            refresh();
        }

        time_t current_time_2 = time(NULL);
        if (current_time_2 - last_log_time >= 1) {
            log_execution(log_file);
            last_log_time = current_time_2;
        }
    }

    endwin();
    close(fd_receive);
    close(fd_ask);
    return 0;
}