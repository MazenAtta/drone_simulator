#include <ncurses.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, ""); // Set locale to UTF-8
    initscr();
    mvaddwstr(0, 0, L"↖ ↑ ↗");
    mvaddwstr(1, 0, L"← ■ →");
    mvaddwstr(2, 0, L"↙ ↓ ↘");
    refresh();
    getch();
    endwin();
    return 0;
}

