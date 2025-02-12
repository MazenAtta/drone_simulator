#ifndef DRONE_SIMULATOR_H
#define DRONE_SIMULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>

// Constants
#define NUM_PROCESSES 4
#define NUM_PIPES 6

// Pipe paths
extern const char *pipes[NUM_PIPES];

// Function declarations
void error_exit(const char *msg);
void create_named_pipe(const char *path);
void handle_stop_start(int pid[]);
void handle_terminate();
void handle_reset(int pid[], const char *process_args[][6]);

// Signal handlers
extern volatile sig_atomic_t running;

#endif // DRONE_SIMULATOR_H