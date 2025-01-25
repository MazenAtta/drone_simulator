#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define COLS 80
#define LINES 25
#define MAX_TARGETS 5
#define BORDER_MARGIN 2

typedef struct {
    int x[MAX_TARGETS], y[MAX_TARGETS];
    int id[MAX_TARGETS];
} Target;

void init_targets(Target *targets) {
    for (int i = 0; i < MAX_TARGETS; i++) {
        targets->x[i] = -1; // Off-screen by default
        targets->y[i] = -1;
        targets->id[i] = i + 1;
    }
}

void generate_targets(Target *targets) {
    for (int i = 0; i < MAX_TARGETS; i++) {
        targets->x[i] = BORDER_MARGIN + rand() % (COLS - 2 * BORDER_MARGIN);
        targets->y[i] = BORDER_MARGIN + rand() % (LINES - 2 * BORDER_MARGIN);
    }
}

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    const char *target_receive = "/tmp/target_receive";

    int fd_receive = open(target_receive, O_WRONLY);
    if (fd_receive < 0) {
        error_exit("Failed to open named pipe");
    }

    Target targets;
    init_targets(&targets);

    while (1) {
        generate_targets(&targets);
        // Send the entire Target struct
        if (write(fd_receive, &targets, sizeof(targets)) < 0) {
            error_exit("Failed to write to named pipe");
        }

        sleep(10); // Sleep for 10 seconds before generating new targets
    }

    close(fd_receive);
    return 0;
}