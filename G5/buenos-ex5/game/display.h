#ifndef GAME_DISPLAY
#define GAME_DISPLAY

typedef struct {
    int width;
    int height;

    char *data;
} display_t;

/* initalize a display - must call before use */
void display_init(display_t *display, int width, int height);

/* print the contents of the display to the terminal after clearing */
void display_flip(display_t *display);

/* Set position (x,y) to c */
void display_set(display_t *display, char c, int x, int y);

/* Empty display */
void display_clear(display_t *display);

#endif
