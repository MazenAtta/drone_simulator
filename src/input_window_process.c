#include "input_window_handler.h"

int main() {
    setlocale(LC_ALL, ""); // Set locale to the user's default locale

    const char *input_ask = "/tmp/input_ask";
    const char *input_receive = "/tmp/input_receive";
    const char *input_signal = "/tmp/input_signal";
    const char *log_folder = "log";
    const char *log_file = "log/input_window_log.txt";

    // Create log folder if it doesn't exist
    mkdir(log_folder, 0777);

    int fd_receive = open(input_receive, O_WRONLY | O_NONBLOCK);
    int fd_ask = open(input_ask, O_RDONLY | O_NONBLOCK);
    int fd_signal = open(input_signal, O_WRONLY | O_NONBLOCK);
    Game game;
    initscr();
    start_color(); // Start color functionality
    init_pair(1, COLOR_RED, COLOR_BLACK); // Initialize color pair 1 with red text on black background
    noecho();
    cbreak();
    nodelay(stdscr, TRUE); // Make getch() non-blocking

    input_display();
    display_controls();
    dynamic_display();
    Drone drone;
    display_drone_state(&drone);

    int ch;
    HighlightState highlight_state = {0, 0}; // Initialize the highlight state

    while (1) {
        ch = getch();
        if (ch != ERR) { // If a key was pressed
            switch (ch) {
            case 'b': case 'q': case 'w': case 'e': case 'a': case 's': case 'd': case 'z': case 'x': case 'c' : case 'p': case 'r': case 'k': case 'l':
                if(ch != 'b') game.command = ch;
                else if(ch == 'b') game.game_start = 1;
                write(fd_receive, &game, sizeof(Game));
                highlight_state.button = ch;
                highlight_state.highlight_end = time(NULL) + 1; // Highlight for 1 second
                if (ch == 'p' || ch == 'r' || ch == 'k') {
                write(fd_signal, &ch, sizeof(ch)); // Write to signal buffer
                }
                break;
            default:
                refresh();
            }
        }

        ssize_t bytes_read = read(fd_ask, &drone, sizeof(Drone));
        if (bytes_read > 0) {
            display_drone_state(&drone);
        }

        highlight_button(&highlight_state);
        static time_t last_log_time = 0;
        time_t current_time = time(NULL);
        if (current_time - last_log_time >= 5) {
            log_execution(log_file); // Log execution details
            last_log_time = current_time;
        }    
    }

    endwin();
    close(fd_receive);
    return 0;
}