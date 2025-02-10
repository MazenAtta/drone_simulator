#include "server_handler.hpp"
#include "../DDS/src/CustomSubscriber.hpp"


void convert_from_dds_obstacles(const Obstacles& src, Obstacle& dest)
{
    for (int i = 0; i < MAX_OBSTACLES; ++i)
    {
        dest.x[i] = src.obstacles_x()[i];
        dest.y[i] = src.obstacles_y()[i];
    }
}

void convert_from_dds_targets(const Targets& src, Target& dest)
{
    for (int i = 0; i < MAX_TARGETS; ++i)
    {
        dest.x[i] = src.targets_x()[i];
        dest.y[i] = src.targets_y()[i];
        dest.id[i] = i+1;
    }
}


int main() {
    const char *input_ask = "/tmp/input_ask";
    const char *input_receive = "/tmp/input_receive";
    const char *output_ask = "/tmp/output_ask";
    const char *output_receive = "/tmp/output_receive";
    const char *log_folder = "log";
    const char *log_file = "log/server_log.txt";

    // Create log folder if it doesn't exist
    mkdir(log_folder, 0777);

    // Open named pipes
    int fd_output_ask = open(output_ask, O_WRONLY | O_NONBLOCK);
    int fd_output_receive = open(output_receive, O_RDONLY | O_NONBLOCK);
    int fd_input_ask = open(input_ask, O_WRONLY | O_NONBLOCK);
    int fd_input_receive = open(input_receive, O_RDONLY | O_NONBLOCK);
    
    if (fd_output_ask < 0 || fd_output_receive < 0 || fd_input_ask < 0 || fd_input_receive < 0) {
        error_exit("Failed to open named pipes");
    }

    fd_set read_fds;
    Drone drone;
    Obstacle obstacles;
    Target targets;
    Obstacles dds_obstacles; // DDS message type
    Targets dds_targets; // DDS message type
    Game game;

    // Initialize DDS subscribers
    DDSSubscriber<Obstacles, ObstaclesPubSubType> obstacle_subscriber;
    DDSSubscriber<Targets, TargetsPubSubType> target_subscriber;

    if (!obstacle_subscriber.init("ObstacleTopic")) {
        error_exit("Failed to initialize DDS Obstacle Subscriber");
    }
    if (!target_subscriber.init("TargetTopic")) {
        error_exit("Failed to initialize DDS Target Subscriber");
    }

    obstacle_subscriber.waitPub();
    target_subscriber.waitPub();

        // Determine the maximum file descriptor value for select
    int max_fd = fd_input_receive;
    if (fd_output_receive > max_fd) max_fd = fd_output_receive;

    static time_t last_log_time = 0;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(fd_input_receive, &read_fds);
        FD_SET(fd_output_receive, &read_fds);

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("Select error");
            continue;
        }
        // Receive obstacles and targets from DDS
        const Obstacles* obstacle_data = obstacle_subscriber.get_valid_data();
        if (obstacle_data) {
            // Print the received obstacle data
            printf("Received Obstacles:\n");
            for (int i = 0; i < MAX_OBSTACLES; ++i) {
                printf("Obstacle %d: x = %f, y = %f\n", i, obstacles.x[i], obstacles.y[i]);
            }
        }
        const Targets* target_data = target_subscriber.get_valid_data();


        if (obstacle_data && target_data && (game.game_start == 0)) {
            convert_from_dds_obstacles(*obstacle_data, obstacles);
            convert_from_dds_targets(*target_data, targets);
            update_game_state(&game, &obstacles, &targets);
            write(fd_output_ask, &game, sizeof(Game));
        }

        if (FD_ISSET(fd_input_receive, &read_fds) && game.game_start == 0) {
            read(fd_input_receive, &game, sizeof(Game));
            convert_from_dds_obstacles(*obstacle_data, obstacles);
            convert_from_dds_targets(*target_data, targets);
            update_game_state(&game, &obstacles, &targets);
            if (game.game_start == 1) {
                printf("Game started\n");
            }   
        }

        if (FD_ISSET(fd_output_receive, &read_fds) && game.game_start == 1) {
            ssize_t bytes_read = read(fd_output_receive, &drone, sizeof(Drone));
            if (bytes_read > 0) {
                if (drone.score != 0){game.score = drone.score;}
                game.game_update = 0;
                game.command = 0;
                write(fd_input_ask, &drone, sizeof(Drone));
            } else {
                perror("Error reading from fd_output_receive");
            }
        }

        if (game.game_start == 1 && (FD_ISSET(fd_input_receive, &read_fds) || obstacle_data || target_data)) {
            if (FD_ISSET(fd_input_receive, &read_fds)) {
                read(fd_input_receive, &game, sizeof(Game));
            } else if (obstacle_data) {
                convert_from_dds_obstacles(*obstacle_data, obstacles);
                game.game_update = 1;
            } else if (target_data && game.score % 5 == 0 && game.score > 0) {
                convert_from_dds_targets(*target_data, targets);
                game.game_update = 2;
                game.score = 0;
            }
            update_game_state(&game, &obstacles, &targets);
            write(fd_output_ask, &game, sizeof(Game));
        }

        time_t current_time = time(NULL);
        if (current_time - last_log_time >= 1) {
            log_execution(log_file); // Log execution details
            last_log_time = current_time;
        }
    }

    // Close named pipes
    close(fd_input_receive);
    close(fd_output_receive);
    close(fd_output_ask);
    close(fd_input_ask);

    return 0;
}