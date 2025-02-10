#include "input_window_handler.hpp"

int main() {
    setlocale(LC_ALL, ""); // Set locale to the user's default locale

    const char *input_ask = "/tmp/input_ask";
    const char *input_receive = "/tmp/input_receive";
    const char *input_signal = "/tmp/input_signal";
    const char *log_folder = "log";
    const char *log_file = "log/input_window_log.txt";

    // Create log folder if it doesn't exist
    mkdir(log_folder, 0777);

    // Open file descriptors for communication
    int fd_receive = open(input_receive, O_WRONLY | O_NONBLOCK);
    int fd_ask = open(input_ask, O_RDONLY | O_NONBLOCK);
    int fd_signal = open(input_signal, O_WRONLY | O_NONBLOCK);
    if (fd_receive == -1 || fd_ask == -1 || fd_signal == -1) {
        perror("Failed to open file descriptors");
        return 1;
    }

    Game game;
    // Initialize ncurses
    if (initscr() == NULL) {
        perror("Failed to initialize ncurses");
        return 1;
    }
    start_color(); // Start color functionality
    init_pair(1, COLOR_RED, COLOR_BLACK); // Initialize color pair 1 with red text on black background
    noecho(); // Disable echoing of typed characters
    cbreak(); // Disable line buffering
    nodelay(stdscr, TRUE); // Make getch() non-blocking

    // Display initial UI components
    input_display();
    display_controls();
    dynamic_display();
    Drone drone;
    display_drone_state(&drone);

    int ch;
    HighlightState highlight_state = {0, 0}; // Initialize the highlight state
    time_t last_log_time = 0;

    // Main loop
    while (1) {
        ch = getch();
        if (ch != ERR) { // If a key was pressed
            switch (ch) {
                case 'b': case 'q': case 'w': case 'e': case 'a': case 's': case 'd': case 'z': case 'x': case 'c': case 'p': case 'r': case 'k': case 'l':
                    if (ch != 'b') game.command = ch;
                    else if (ch == 'b') {
                        memset(&game, 0, sizeof(Game)); // Set the whole game struct to 0
                        game.game_start = 1;
                    }
                    write(fd_receive, &game, sizeof(Game)); // Send game command
                    highlight_state.button = ch;
                    highlight_state.highlight_end = time(NULL) + 1; // Highlight for 1 second
                    if (ch == 'p' || ch == 'r' || ch == 'k') {
                        write(fd_signal, &ch, sizeof(ch)); // Write to signal buffer
                    }
                    break;
                default:
                    refresh(); // Refresh the screen
            }
        }
        else if (game.game_start == 0) {
            memset(&game, 0, sizeof(Game)); // Set the whole game struct to 0
            write(fd_receive, &game, sizeof(Game)); // Send game command
        }

        // Read drone state from input_ask
        ssize_t bytes_read = read(fd_ask, &drone, sizeof(Drone));
        if (bytes_read > 0) {
            display_drone_state(&drone); // Update drone state display
        }

        highlight_button(&highlight_state); // Highlight button if needed
        time_t current_time = time(NULL);
        if (current_time - last_log_time >= 1) {
            log_execution(log_file); // Log execution details
            last_log_time = current_time;
        }
    }

    // Cleanup
    endwin();
    close(fd_receive);
    close(fd_ask);
    close(fd_signal);
    return 0;
}