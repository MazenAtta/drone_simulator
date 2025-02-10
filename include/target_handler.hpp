#ifndef TARGET_HANDLER_H
#define TARGET_HANDLER_H

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define COLS 80
#define LINES 25
#define MAX_TARGETS 5
#define BORDER_MARGIN 2

typedef struct {
    int x[MAX_TARGETS], y[MAX_TARGETS];
    int id[MAX_TARGETS];
} Target;

void init_targets(Target *targets);
void generate_targets(Target *targets);
void error_exit(const char *msg);
void log_execution(const char *log_file);

#endif // TARGET_HANDLER_H