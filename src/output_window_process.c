#include "output_window_handler.h"

int main() {
    const char *output_ask = "/tmp/output_ask";
    const char *output_receive = "/tmp/output_receive";
    const char *log_folder = "log";
    const char *log_file = "log/output_window_log.txt";

    mkdir(log_folder, 0777);

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

    while (1) {
        if (game.game_start == 0) {
            read(fd_ask, &game, sizeof(Game));
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                obstacles.x[i] = game.Obstacle_x[i];
                obstacles.y[i] = game.Obstacle_y[i];
            }
            for (int i = 0; i < MAX_TARGETS; i++) {
                targets.x[i] = game.Target_x[i];
                targets.y[i] = game.Target_y[i];
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
            read(fd_ask, &game, sizeof(Game));

            update_drone_state(&drone, game.command);
            game.command = 0;
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
        static time_t last_log_time = 0;
        time_t current_time = time(NULL);
        if (current_time - last_log_time >= 5) {
            log_execution(log_file);
            last_log_time = current_time;
        }
        usleep(100000);
    }

    endwin();
    close(fd_receive);
    close(fd_ask);
    return 0;
}