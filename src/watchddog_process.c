#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    const char *watchdog_ask = "/tmp/watchdog_ask";

    while (1) {
        // Monitor processes and send notifications if any process stops responding
        sleep(1);
    }

    return 0;
}