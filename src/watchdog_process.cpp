#include "watchdog_handler.hpp"

// Global variable to indicate if the process is paused
volatile sig_atomic_t paused = 0;

// Signal handler for SIGCONT signal
void handle_sigcont(int signum) {
    paused = 1;
}


int main() {
    // Path to the named pipe for watchdog signal
    const char *watchdog_signal = "/tmp/watchdog_signal";
    
    // Open the named pipe for writing in non-blocking mode
    int fd_signal = open(watchdog_signal, O_WRONLY | O_NONBLOCK);
    if (fd_signal < 0) {
        error_exit("Failed to open named pipe");
    }

    // Set up the signal handler for SIGCONT
    signal(SIGCONT, handle_sigcont);

    // Initial sleep before starting the main loop
    sleep(1);
    
    // Main loop
    while (1) {
        // If paused, sleep for 1 second and reset the paused flag
        if (paused) {
            sleep(1);
            paused = 0;
        }
        // Check deadlines using the file descriptor
        check_deadlines(fd_signal);
    }

    // Close the file descriptor before exiting
    close(fd_signal);
    return 0;
}