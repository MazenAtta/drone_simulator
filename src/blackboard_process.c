#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void create_named_pipe(const char *path) {
    if (mkfifo(path, 0666) == -1) {
        perror("mkfifo");
    }
}

// Signal handlers
volatile sig_atomic_t running = 1;

void handle_stop_start(int pid[]) {
    static int stopped = 0;
    if (stopped) {
        for (int i = 0; i < 6; i++) {
            if (i != 1) { // Skip the input window process
                kill(pid[i], SIGCONT); // Continue the process
            }
        }
        stopped = 0;
    } else {
        for (int i = 0; i < 6; i++) {
            if (i != 1) { // Skip the input window process
                kill(pid[i], SIGSTOP); // Stop the process
            }
        }
        stopped = 1;
    }
}

void handle_terminate() {
    running = 0;
    kill(0, SIGTERM); // Terminate all processes
}

void handle_reset(int pid[], const char *process_args[][6]) {
    // Terminate all processes
    for (int i = 0; i < 6; i++) {
        kill(pid[i], SIGTERM);
        waitpid(pid[i], NULL, 0); // Wait for the process to terminate
    }

    // Restart all processes
    for (int i = 0; i < 6; i++) {
        pid_t new_pid = fork();
        if (new_pid == 0) {
            execvp(process_args[i][0], (char *const *)process_args[i]);
            error_exit("Failed to restart process");
        } else if (new_pid > 0) {
            pid[i] = new_pid;
        } else {
            error_exit("Failed to fork process");
        }
    }
}

int main() 
{
    int fd_pid[6];
    // Define paths for named pipes
    const char *input_ask = "/tmp/input_ask";
    const char *input_receive = "/tmp/input_receive";
    const char *input_signal = "/tmp/input_signal";
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
    create_named_pipe(input_signal);
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
    int fd_input_signal = open(input_signal, O_RDWR);
    int fd_output_ask = open(output_ask, O_RDWR);
    int fd_output_receive = open(output_receive, O_RDWR);
    int fd_obstacle_ask = open(obstacle_ask, O_RDWR);
    int fd_obstacle_receive = open(obstacle_receive, O_RDWR);
    int fd_target_ask = open(target_ask, O_RDWR);
    int fd_target_receive = open(target_receive, O_RDWR);
    int fd_watchdog_ask = open(watchdog_ask, O_RDWR);
    int fd_watchdog_receive = open(watchdog_receive, O_RDWR);

    // Define the arguments for each process
    const char *process_args[6][6] = {
            {"./server_process", NULL, NULL, NULL, NULL, NULL},
            {"konsole", "--geometry", "650x400", "-e", "./input_window_process", NULL},
            {"konsole", "--geometry", "830x565", "-e", "./output_window_process", NULL},
            {"./obstacle_process", NULL, NULL, NULL, NULL, NULL},
            {"./target_process", NULL, NULL, NULL, NULL, NULL},
            {"./watchdog_process", NULL, NULL, NULL, NULL, NULL}
        };

    // Spawn the processes using the process_args array
    for (int i = 0; i < 6; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execvp(process_args[i][0], (char *const *)process_args[i]);
            error_exit("Failed to spawn process");
        } else if (pid > 0) {
            fd_pid[i] = pid;
        } else {
            error_exit("Failed to fork process");
        }
    }

    char buffer[2];
    ssize_t bytes_read;

    while ((bytes_read = read(fd_input_signal, buffer, 1)) > 0) {
        buffer[bytes_read] = '\0';
        if (buffer[0] == 'p') {
            handle_stop_start(fd_pid);
        } else if (buffer[0] == 'r') {

            handle_reset(fd_pid, process_args);
        } else if (buffer[0] == 'k') {
            handle_terminate();
        }
    }

    if (bytes_read == -1) {
        perror("read");
    }

    // Wait for all child processes to finish
    for (int i = 0; i < 6; i++) {
        waitpid(fd_pid[i], NULL, 0);
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