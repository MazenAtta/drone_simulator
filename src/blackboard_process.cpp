#include "blackboard_handler.hpp"

int main() {
    int fd_pid[NUM_PROCESSES];

    // Create named pipes
    for (int i = 0; i < NUM_PIPES; i++) {
        create_named_pipe(pipes[i]);
    }

    // Open named pipes in the blackboard process
    int fds[NUM_PIPES];
    for (int i = 0; i < NUM_PIPES; i++) {
        fds[i] = open(pipes[i], O_RDWR);
        if (fds[i] == -1) {
            error_exit("Failed to open named pipe");
        }
    }

    // Define the arguments for each process
    const char *process_args[NUM_PROCESSES][6] = {
        {"./build/server_process", NULL, NULL, NULL, NULL, NULL},
        {"konsole", "--geometry", "650x400", "-e", "./build/input_window_process", NULL},
        {"konsole", "--geometry", "830x565", "-e", "./build/output_window_process", NULL},
        {"./build/watchdog_process", NULL, NULL, NULL, NULL, NULL}
    };

    // Spawn the processes using the process_args array
    for (int i = 0; i < NUM_PROCESSES; i++) {
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

    fd_set read_fds;
    int max_fd = (fds[2] > fds[5]) ? fds[2] : fds[5];

    while (running) {
        FD_ZERO(&read_fds);
        FD_SET(fds[2], &read_fds); // input_signal
        FD_SET(fds[5], &read_fds); // watchdog_signal

        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if (activity == -1) {
            perror("select");
            break;
        }

        if (FD_ISSET(fds[2], &read_fds)) {
            bytes_read = read(fds[2], buffer, 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                if (buffer[0] == 'p') {
                    handle_stop_start(fd_pid);
                } else if (buffer[0] == 'r') {
                    handle_reset(fd_pid, process_args);
                } else if (buffer[0] == 'k') {
                    handle_terminate();
                }
            } else if (bytes_read == -1) {
                perror("read fd_input_signal");
            }
        }

        if (FD_ISSET(fds[5], &read_fds)) {
            bytes_read = read(fds[5], buffer, 1);
            if (bytes_read > 0) {
                handle_terminate();
            } else if (bytes_read == -1) {
                perror("read fd_watchdog_signal");
            }
        }
    }

    if (bytes_read == -1) {
        perror("read");
    }

    // Wait for all child processes to finish
    for (int i = 0; i < NUM_PROCESSES; i++) {
        waitpid(fd_pid[i], NULL, 0);
    }

    // Close and remove named pipes
    for (int i = 0; i < NUM_PIPES; i++) {
        close(fds[i]);
        unlink(pipes[i]);
    }

    return 0;
}