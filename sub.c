#include <ncurses.h>

void draw_L(int y, int x) {
    for (int i = 0; i < 5; i++) mvprintw(y + i, x, "|");
    mvprintw(y + 5, x, "-------------------");
}

void draw_O(int y, int x) {
    mvprintw(y, x, "-----------------");
    for (int i = 1; i < 5; i++) {
        mvprintw(y + i, x, "|               |");
    }
    mvprintw(y + 5, x, "-----------------");
}

void draw_S(int y, int x) {
    mvprintw(y, x, " ---------------- ");
    mvprintw(y + 1, x, "|");
    mvprintw(y + 2, x, " ---------------- ");
    mvprintw(y + 3, x + 17, "|");
    mvprintw(y + 4, x, " ---------------- ");
}

void draw_E(int y, int x) {
    mvprintw(y, x, " ---------------- ");
    for (int i = 1; i < 4; i++) {
        mvprintw(y + i, x, "|");
    }
    mvprintw(y + 2, x, " ---------------- ");
    for (int i = 3; i < 5; i++) {
        mvprintw(y + i, x, "|");
    }
    mvprintw(y + 5, x, " ---------------- ");
}

void draw_Exclam(int y, int x) {
    for (int i = 0; i < 4; i++) mvprintw(y + i, x, "|");
    mvprintw(y + 4, x, " ");
    mvprintw(y + 5, x, "|");
}

int main() {
    initscr();  
    noecho();  
    curs_set(0);  

    int start_y = 5, start_x = 10;
    
    draw_L(start_y, start_x);
    draw_O(start_y, start_x + 25);
    draw_S(start_y, start_x + 50);
    draw_E(start_y, start_x + 75);
    draw_Exclam(start_y, start_x + 100);

    refresh();  
    getch();    

    endwin();   
    return 0;
}
