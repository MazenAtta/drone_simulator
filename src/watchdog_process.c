#include "watchdog_handler.h"

int main() {
    const char *input_signal = "/tmp/input_signal";
    int fd_signal = open(input_signal, O_WRONLY | O_NONBLOCK);
    if (fd_signal < 0) {
        error_exit("Failed to open named pipe");
    }

    printf("Starting watchdog process\n");
    sleep(1);
    while (1) {
        check_deadlines(fd_signal);
        sleep(1); // Add a small delay to avoid busy-waiting
    }

    close(fd_signal);
    return 0;
}