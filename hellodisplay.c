#include "display.h"
#include <stdbool.h>

#define SCALE 16
typedef struct location {
    int x, y;
} location;

bool functionToCallRepeatedly(display *d, void *data, SDL_Keycode pressedKey) {
    location *l = (location *)data;
    colour(d, 0xFF);
    block(d, 0, 0, getWidth(d), getHeight(d));
    switch (pressedKey) {
    case SDLK_RIGHT:
        l->x += SCALE;
        break;
    case SDLK_LEFT:
        l->x -= SCALE;
        break;
    case SDLK_DOWN:
        l->y += SCALE;
        break;
    case SDLK_UP:
        l->y -= SCALE;
    }
    *l = (location){(l->x + getWidth(d)) % getWidth(d), (l->y + getHeight(d)) % getHeight(d)};
    colour(d, 0xFFFF);
    block(d, l->x, l->y, SCALE, SCALE);
    show(d);
    return pressedKey == SDLK_ESCAPE;
}

int main() {
    display *d = newDisplay("Hello Display", 320, 240);
    if (d == NULL)
        return 1;
    location *l = malloc(sizeof(location));
    *l = (location){0, 0};
    run(d, l, functionToCallRepeatedly);
    free(l);
    freeDisplay(d);
    return 0;
}
