#include "blackboard_handler.h"

// Pipe paths
const char *pipes[NUM_PIPES] = {
    "/tmp/input_ask", "/tmp/input_receive", "/tmp/input_signal",
    "/tmp/output_ask", "/tmp/output_receive", "/tmp/obstacle_ask",
    "/tmp/obstacle_receive", "/tmp/target_ask", "/tmp/target_receive",
    "/tmp/watchdog_signal"
};

// Signal handlers
volatile sig_atomic_t running = 1;

// Function to print error message and exit
void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Function to create named pipe
void create_named_pipe(const char *path) {
    if (mkfifo(path, 0666) == -1) {
        perror("mkfifo");
    }
}

// Function to handle stop/start signal
void handle_stop_start(int pid[]) {
    static int stopped = 0;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (i != 1) { // Skip the input window process
            kill(pid[i], stopped ? SIGCONT : SIGSTOP); // Toggle between stop and continue
        }
    }
    stopped = !stopped;
}

// Function to handle terminate signal
void handle_terminate() {
    running = 0;
    kill(0, SIGTERM); // Terminate all processes
}

// Function to handle reset signal
void handle_reset(int pid[], const char *process_args[][6]) {
    // Terminate all processes
    for (int i = 0; i < NUM_PROCESSES; i++) {
        kill(pid[i], SIGTERM);
        waitpid(pid[i], NULL, 0); // Wait for the process to terminate
    }

    // Restart all processes
    for (int i = 0; i < NUM_PROCESSES; i++) {
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