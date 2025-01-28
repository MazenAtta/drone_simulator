#include "server_handler.h"

int main() {
    const char *input_ask = "/tmp/input_ask";
    const char *input_receive = "/tmp/input_receive";
    const char *output_ask = "/tmp/output_ask";
    const char *output_receive = "/tmp/output_receive";
    const char *obstacle_receive = "/tmp/obstacle_receive";
    const char *target_receive = "/tmp/target_receive";
    const char *log_folder = "log";
    const char *log_file = "log/server_log.txt";

    // Create log folder if it doesn't exist
    mkdir(log_folder, 0777);

    // Open named pipes
    int fd_output_ask = open(output_ask, O_WRONLY | O_NONBLOCK);
    int fd_output_receive = open(output_receive, O_RDONLY | O_NONBLOCK);
    int fd_input_ask = open(input_ask, O_WRONLY | O_NONBLOCK);
    int fd_input_receive = open(input_receive, O_RDONLY | O_NONBLOCK);
    int fd_obstacle_receive = open(obstacle_receive, O_RDONLY | O_NONBLOCK);
    int fd_target_receive = open(target_receive, O_RDONLY | O_NONBLOCK);
    
    if (fd_output_ask < 0 || fd_output_receive < 0 || fd_input_ask < 0 || fd_input_receive < 0 || fd_obstacle_receive < 0 || fd_target_receive < 0) {
        error_exit("Failed to open named pipes");
    }

    fd_set read_fds;
    Drone drone;
    Obstacle obstacles;
    Target targets;
    Game game;

    game.game_start = 0;
    // Determine the maximum file descriptor value for select
    int max_fd = fd_input_receive;
    if (fd_output_receive > max_fd) max_fd = fd_output_receive;
    if (fd_obstacle_receive > max_fd) max_fd = fd_obstacle_receive;
    if (fd_target_receive > max_fd) max_fd = fd_target_receive;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(fd_input_receive, &read_fds);
        FD_SET(fd_output_receive, &read_fds);
        FD_SET(fd_obstacle_receive, &read_fds);
        FD_SET(fd_target_receive, &read_fds);

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("Select error");
            continue;
        }

        if (FD_ISSET(fd_obstacle_receive, &read_fds) && FD_ISSET(fd_target_receive, &read_fds) && game.game_start == 0) {
            read(fd_obstacle_receive, &obstacles, sizeof(Obstacle));
            read(fd_target_receive, &targets, sizeof(Target));
            // Update game state
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                game.Obstacle_x[i] = obstacles.x[i];
                game.Obstacle_y[i] = obstacles.y[i];
            }
            for (int i = 0; i < MAX_TARGETS; i++) {
                game.Target_x[i] = targets.x[i];
                game.Target_y[i] = targets.y[i];
                game.target_id[i] = targets.id[i];
            }
            write(fd_output_ask, &game, sizeof(Game));
        }

        if (FD_ISSET(fd_input_receive, &read_fds) && game.game_start == 0) {
            read(fd_input_receive, &game, sizeof(Game));
            // Update game state
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                game.Obstacle_x[i] = obstacles.x[i];
                game.Obstacle_y[i] = obstacles.y[i];
            }
            for (int i = 0; i < MAX_TARGETS; i++) {
                game.Target_x[i] = targets.x[i];
                game.Target_y[i] = targets.y[i];
                game.target_id[i] = targets.id[i];
            }
            if (game.game_start == 1) {
                printf("Game started\n");
            }   
        }

        if (FD_ISSET(fd_input_receive, &read_fds) && game.game_start == 1) {
            ssize_t bytes_read = read(fd_input_receive, &game, sizeof(Game));
            // Update game state
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                game.Obstacle_x[i] = obstacles.x[i];
                game.Obstacle_y[i] = obstacles.y[i];
            }
            for (int i = 0; i < MAX_TARGETS; i++) {
                game.Target_x[i] = targets.x[i];
                game.Target_y[i] = targets.y[i];
                game.target_id[i] = targets.id[i];
            }
            if (bytes_read > 0) {
                write(fd_output_ask, &game, sizeof(Game)); // Send command to output window
            } else {
                perror("Error reading from fd_input_receive");
            }
        }

        if (FD_ISSET(fd_output_receive, &read_fds) && game.game_start == 1) {
            ssize_t bytes_read = read(fd_output_receive, &drone, sizeof(Drone));
            if (bytes_read > 0) {
                write(fd_input_ask, &drone, sizeof(Drone));
            } else {
                perror("Error reading from fd_output_receive");
            }
        }

        static time_t last_log_time = 0;
        time_t current_time = time(NULL);
        if (current_time - last_log_time >= 5) {
            log_execution(log_file); // Log execution details
            last_log_time = current_time;
        }
    }

    // Close named pipes
    close(fd_input_receive);
    close(fd_output_receive);
    close(fd_output_ask);
    close(fd_input_ask);
    close(fd_obstacle_receive);
    close(fd_target_receive);

    return 0;
}