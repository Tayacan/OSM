/*
 * Cursor control codes
 * */

#include "game/cursor.h"
#include "tests/lib.h"

/* Clear the screen */
void clear_screen(void) {
    puts("\033[2J\033[0;0H");
}

/* Set cursor position */
void cursor_set(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

