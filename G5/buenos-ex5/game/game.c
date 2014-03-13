#include "tests/lib.h"
#include "game/display.h"
#include "game/config.h"

#define MIDDLE 7
int x = MIDDLE;

void move(int dx) {
    x += dx;
    if(x < 1) x = 1;
    if(x >= WIDTH-1) x = WIDTH-2;
}

int main(void) {
    display_t disp;
    char c;
    int i,j;
    int lowest[WIDTH-2];
    char player = 'x';
    heap_init();

    display_init(&disp, WIDTH, HEIGHT);

    // Put in the walls
    for(i = 1; i < disp.width-1; i++) {
        display_set(&disp, '-', i, 1);
        display_set(&disp, '-', i, disp.height - 1);
    }
    for(i = 0; i < disp.height; i++) {
        display_set(&disp, '|', 0, i);
        display_set(&disp, '|', disp.width - 1, i);
    }
    for(i = 2; i <= disp.width-3; i = i + 2) {
        for(j = 2; j < disp.height-1; j++) {
            display_set(&disp, '|', i, j);
        }
    }

    // Lowest is HEIGHT-2 everywhere
    for(i = 0; i < WIDTH-2; i++) {
        lowest[i] = HEIGHT-2;
    }

    // Place player and show!
    display_set(&disp, player, x, 0);
    display_flip(&disp);
    while((c = getc_raw()) != 'q') {
        display_set(&disp, ' ', x, 0);
        if(c == 'a') {
            move(-2);
        } else if(c == 'd') {
            move(2);
        } else if(c == ' ') {
            // place marker
            i = (x-1) / 2;
            display_set(&disp, player, x, lowest[i]);
            lowest[i]--;
            // switch player
            if(player == 'x') player = 'o';
            else player = 'x';
            // reset player pos
            display_set(&disp, ' ', x, 0);
            x = MIDDLE;
        }
        display_set(&disp, player, x, 0);
        display_flip(&disp);
    }

    syscall_exit(0);
    return 0;
}

