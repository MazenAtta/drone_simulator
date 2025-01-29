#ifndef WATCHDOG_HANDLER_H
#define WATCHDOG_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#define LOG_FILES_COUNT 5
#define TIMEOUT 5

extern const char *log_files[LOG_FILES_COUNT];

void error_exit(const char *msg);
void check_deadlines(int fd_signal);

#endif // WATCHDOG_HANDLER_H