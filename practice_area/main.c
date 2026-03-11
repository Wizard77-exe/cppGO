#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LEN 256

int main() {
    char command[MAX_LINE_LEN];
    char func[50];
    char operator;
    float num1, num2, result;

    // initialize ncurses
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // prompt green
    cbreak();
    echo();

    while (1) {
        // display prompt
        attron(COLOR_PAIR(1));
        printw("prompt::> ");
        attroff(COLOR_PAIR(1));
        refresh();

        // read input
        getnstr(command, MAX_LINE_LEN);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0) {
            printw("GOODBYE!\n");
            refresh();
            break;
        }

        if (sscanf(command, "%s %f %c %f", func, &num1, &operator, &num2) != 4) {
            printw("FATAL ERROR: Undefined Behaviour!\n");
            continue;
        }

        if (strcmp(func, "shout") == 0) {
            switch (operator) {
                case '+': result = num1 + num2; break;
                case '-': result = num1 - num2; break;
                case '*': result = num1 * num2; break;
                case '/':
                    if (num2 != 0) result = num1 / num2;
                    else { printw("FATAL ERROR: Division By Zero!\n"); continue; }
                    break;
                default: printw("FATAL ERROR: Syntax Error!\n"); continue;
            }
            // print result, integer if whole
            if ((int)result == result) printw("%d\n", (int)result);
            else printw("%f\n", result);
        } else {
            printw("FATAL ERROR: Syntax Error!\n");
        }
        refresh();
    }

    endwin(); // cleanup ncurses
    return 0;
}