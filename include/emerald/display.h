#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct display;
typedef struct display display;

typedef bool (*displayAction)(display *display, void *data, SDL_Keycode key);

display *newDisplay(const char *title, int width, int height);

int getWidth(display *d);
int getHeight(display *d);

void freeDisplay(display *d);
void show(display *d);
void line(display *d, int x0, int y0, int x1, int y1);
void block(display *d, int x, int y, int w, int h);
void pixel(display *d, int x, int y);
void colour(display *d, Uint32 rgba);
void run(display *d, void *data, displayAction action);
