#include "game/display.h"
#include "game/cursor.h"
#include "tests/lib.h"
#include "game/config.h"

/* initalize a display - must call before use */
void display_init(display_t *display, int width, int height) {
    display->width = width;
    display->height = height;
    display->data = calloc(sizeof(char), width * height);
    display_clear(display);
}

/* print the contents of the display to the terminal after clearing */
void display_flip(display_t *display) {
    int i, j;

    clear_screen();
    for(i = 0; i < display->height; i++) {
        for(j = 0; j < display->width; j++) {
            putc(display->data[i * display->width + j]);
        }
        puts("\n");
    }
}

void display_clear(display_t *display) {
    int i;
    for(i = 0; i < display->width * display->height; i++) {
        display->data[i] = ' ';
    }
}

/* Set position (x,y) to c */
void display_set(display_t *display, char c, int x, int y) {
    if(x < 0 || x >= display->width || y < 0 || y >= display->height) {
        return;
    }
    display->data[y * display->width + x] = c;
}
