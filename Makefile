# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -D_XOPEN_SOURCE_EXTENDED -Iinclude
LDFLAGS = -lncursesw -lm
# Directories
SRCDIR = src
INCDIR = include

# Source files
BLACKBOARD_SRC = $(SRCDIR)/blackboard_process.c
SERVER_SRC = $(SRCDIR)/server_process.c $(SRCDIR)/physics_handler.c $(SRCDIR)/obstacle_target_handler.c
INPUT_SRC = $(SRCDIR)/input_window_process.c
OUTPUT_SRC = $(SRCDIR)/output_window_process.c $(SRCDIR)/physics_handler.c $(SRCDIR)/obstacle_target_handler.c
OBSTACLE_SRC = $(SRCDIR)/obstacle_process.c $(SRCDIR)/obstacle_target_handler.c
TARGET_SRC = $(SRCDIR)/target_process.c $(SRCDIR)/obstacle_target_handler.c
WATCHDOG_SRC = $(SRCDIR)/watchdog_process.c


# Executables
BLACKBOARD_EXEC = blackboard_process
SERVER_EXEC = server_process
INPUT_EXEC = input_window_process
OUTPUT_EXEC = output_window_process
OBSTACLE_EXEC = obstacle_process
TARGET_EXEC = target_process
WATCHDOG_EXEC = watchdog_process



# All target
all: $(BLACKBOARD_EXEC) $(SERVER_EXEC) $(INPUT_EXEC) $(OUTPUT_EXEC) $(OBSTACLE_EXEC) $(TARGET_EXEC) $(WATCHDOG_EXEC)

# Build individual executables
$(BLACKBOARD_EXEC): $(BLACKBOARD_SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SERVER_EXEC): $(SERVER_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(INPUT_EXEC): $(INPUT_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(OUTPUT_EXEC): $(OUTPUT_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(OBSTACLE_EXEC): $(OBSTACLE_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(TARGET_EXEC): $(TARGET_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(WATCHDOG_EXEC): $(WATCHDOG_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Clean target
clean:
	rm -f $(BLACKBOARD_EXEC) $(SERVER_EXEC) $(INPUT_EXEC) $(OUTPUT_EXEC) $(OBSTACLE_EXEC) $(TARGET_EXEC) $(WATCHDOG_EXEC) $(SRCDIR)/*.o
