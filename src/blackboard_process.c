#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void create_named_pipe(const char *path) {
    if (mkfifo(path, 0666) == -1) {
        perror("mkfifo");
    }
}

int main() 
{

    // Define paths for named pipes
    const char *input_ask = "/tmp/input_ask";
    const char *input_receive = "/tmp/input_receive";
    const char *output_ask = "/tmp/output_ask";
    const char *output_receive = "/tmp/output_receive";
    const char *obstacle_ask = "/tmp/obstacle_ask";
    const char *obstacle_receive = "/tmp/obstacle_receive";
    const char *target_ask = "/tmp/target_ask";
    const char *target_receive = "/tmp/target_receive";
    const char *watchdog_ask = "/tmp/watchdog_ask";
    const char *watchdog_receive = "/tmp/watchdog_receive";

    // Create named pipes
    create_named_pipe(input_ask);
    create_named_pipe(input_receive);
    create_named_pipe(output_ask);
    create_named_pipe(output_receive);
    create_named_pipe(obstacle_ask);
    create_named_pipe(obstacle_receive);
    create_named_pipe(target_ask);
    create_named_pipe(target_receive);
    create_named_pipe(watchdog_ask);
    create_named_pipe(watchdog_receive);

    // Open named pipes in the blackboard process
    int fd_input_ask = open(input_ask, O_RDWR);
    int fd_input_receive = open(input_receive, O_RDWR);
    int fd_output_ask = open(output_ask, O_RDWR);
    int fd_output_receive = open(output_receive, O_RDWR);
    int fd_obstacle_ask = open(obstacle_ask, O_RDWR);
    int fd_obstacle_receive = open(obstacle_receive, O_RDWR);
    int fd_target_ask = open(target_ask, O_RDWR);
    int fd_target_receive = open(target_receive, O_RDWR);
    int fd_watchdog_ask = open(watchdog_ask, O_RDWR);
    int fd_watchdog_receive = open(watchdog_receive, O_RDWR);


    // Spawn the server process
    char *server_process_arg_list[] = {"./server_process", NULL};
    if (fork() == 0) {
        execvp(server_process_arg_list[0], server_process_arg_list);
        error_exit("Failed to spawn server process");
    }                     


    // Spawn the input process in a new terminal window with specific size
    char *input_process_arg_list[] = {"konsole", "--geometry", "650x400", "-e", "./input_window_process", NULL};
    if (fork() == 0) {
        execvp(input_process_arg_list[0], input_process_arg_list);
        error_exit("Failed to spawn input window process");
    }

    // Spawn the output process in a new terminal window with specific size
    char *output_process_arg_list[] = {"konsole", "--geometry", "830x565", "-e", "./output_window_process", NULL};
    if (fork() == 0) {
        execvp(output_process_arg_list[0], output_process_arg_list);
        error_exit("Failed to spawn output window process");
    }

    // Spawn the obstacle process
    char *obstacle_process_arg_list[] = {"./obstacle_process", NULL};
    if (fork() == 0) {
        execvp(obstacle_process_arg_list[0], obstacle_process_arg_list);
        error_exit("Failed to spawn obstacle window process");
    }

    // Spawn the target process in a new terminal window
    char *target_process_arg_list[] = {"./target_process", NULL};
    if (fork() == 0) {
        execvp(target_process_arg_list[0], target_process_arg_list);
        error_exit("Failed to spawn target window process");
    }

    // Spawn the watchdog process
    char *watchdog_process_arg_list[] = {"./watchdog_process", NULL};
    if (fork() == 0) {
        execvp(watchdog_process_arg_list[0], watchdog_process_arg_list);
        error_exit("Failed to spawn watchdog window process");
    }

    // Wait for all child processes to finish
    for (int i = 0; i < 6; i++) {
        wait(NULL);
    }

    // Close and remove named pipes
    close(fd_input_ask);
    close(fd_input_receive);
    close(fd_output_ask);
    close(fd_output_receive);
    close(fd_obstacle_ask);
    close(fd_obstacle_receive);
    close(fd_target_ask);
    close(fd_target_receive);
    close(fd_watchdog_ask);
    close(fd_watchdog_receive);


    // Remove named pipes
    unlink(input_ask);
    unlink(input_receive);
    unlink(output_ask);
    unlink(output_receive);
    unlink(obstacle_ask);
    unlink(obstacle_receive);
    unlink(target_ask);
    unlink(target_receive);
    unlink(watchdog_ask);
    unlink(watchdog_receive);

    return 0;
}