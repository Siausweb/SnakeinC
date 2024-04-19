#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

#define WIDTH 20
#define HEIGHT 20
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 3
#define MOVE_RIGHT 4

int gameover;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

void Setup() {
    initscr(); // Initialize ncurses
    clear(); // Clear the screen
    noecho(); // Don't echo user input
    curs_set(0); // Hide the cursor
    keypad(stdscr, TRUE); // Enable keypad input
    timeout(0); // Non-blocking input

    gameover = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
}

void Draw() {
    clear(); // Clear the screen

    // Draw borders
    for (int i = 0; i < WIDTH + 2; i++)
        mvprintw(0, i, "#");
    for (int i = 0; i < HEIGHT; i++) {
        mvprintw(i + 1, 0, "#");
        mvprintw(i + 1, WIDTH + 1, "#");
    }
    for (int i = 0; i < WIDTH + 2; i++)
        mvprintw(HEIGHT + 1, i, "#");

    // Draw snake and fruit
    mvprintw(y + 1, x + 1, "O");
    mvprintw(fruitY + 1, fruitX + 1, "F");

    // Draw tail
    for (int i = 0; i < nTail; i++)
        mvprintw(tailY[i] + 1, tailX[i] + 1, "o");

    // Draw score
    mvprintw(HEIGHT + 3, 0, "Score: %d", score);

    refresh(); // Refresh the screen
}

void Input() {
    int ch = wgetch(stdscr); // Get user input
    switch (ch) {
        case ERR:
            break; // No input
        case KEY_LEFT:
            dir = LEFT;
            break;
        case KEY_RIGHT:
            dir = RIGHT;
            break;
        case KEY_UP:
            dir = UP;
            break;
        case KEY_DOWN:
            dir = DOWN;
            break;
        case 'x':
            gameover = 1;
            break;
    }
}


void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }
    if (x >= WIDTH) x = 0; else if (x < 0) x = WIDTH - 1;
    if (y >= HEIGHT) y = 0; else if (y < 0) y = HEIGHT - 1;

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameover = 1;
    }
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }
}

int main() {
    srand((unsigned int)time(NULL));
    Setup();
    nodelay(stdscr, TRUE); // Enable non-blocking input
    while (!gameover) {
        Draw();
        usleep(100000); // Adjust the sleep time for controlling game speed
        Input();
        Logic();
    }
    endwin(); // End ncurses mode
    return 0;
}
