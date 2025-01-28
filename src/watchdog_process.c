#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "watchdog_handler.h"

volatile sig_atomic_t paused = 0;

void handle_sigcont() {
    paused = 1;
}

int main() {
    const char *input_signal = "/tmp/input_signal";
    int fd_signal = open(input_signal, O_WRONLY | O_NONBLOCK);
    if (fd_signal < 0) {
        error_exit("Failed to open named pipe");
    }

    signal(SIGCONT, handle_sigcont);

    sleep(1);
    while (1) {
        if (paused) {
            sleep(1);
            paused = 0;
        }
        check_deadlines(fd_signal);
    }

    close(fd_signal);
    return 0;
}