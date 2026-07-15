#include "emerald/display.h"
#include <stdbool.h>

enum { scale = 16 };
typedef struct location {
    int x, y;
} location;

bool functionToCallRepeatedly(display *d, void *data, SDL_Keycode pressedKey) {
    location *l = (location *)data;
    colour(d, 0xFF);
    block(d, 0, 0, getWidth(d), getHeight(d));
    switch (pressedKey) {
    case SDLK_RIGHT:
        l->x += scale;
        break;
    case SDLK_LEFT:
        l->x -= scale;
        break;
    case SDLK_DOWN:
        l->y += scale;
        break;
    case SDLK_UP:
        l->y -= scale;
    }
    *l = (location){(l->x + getWidth(d)) % getWidth(d), (l->y + getHeight(d)) % getHeight(d)};
    colour(d, 0xFFFF);
    block(d, l->x, l->y, scale, scale);
    show(d);
    return pressedKey == SDLK_ESCAPE;
}

int main(void) {
    display *d = newDisplay("Hello Display", 320, 240);
    if (d == NULL)
        return 1;
    location current = {0, 0};
    run(d, &current, functionToCallRepeatedly);
    freeDisplay(d);
    return 0;
}
