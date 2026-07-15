#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct display;
typedef struct display display;

display *newDisplay(char *title, int width, int height);

int getWidth(display *d);
int getHeight(display *d);

void freeDisplay(display *d);
void show(display *d);
void line(display *d, int x0, int y0, int x1, int y1);
void block(display *d, int x, int y, int w, int h);
void pixel(display *d, int x, int y);
void colour(display *d, int rgba);
void run(display *d, void *data, bool action(display *, void *, const char));
