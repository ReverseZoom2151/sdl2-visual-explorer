#include "display.h"
#define SDL_MAIN_HANDLED
#define FAILURE_CODE 1

struct display {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width;
    int height;
};

static void fail() {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    SDL_Quit();
    exit(FAILURE_CODE);
}

static int safeI(int n) {
    if (n < 0)
        fail();
    return n;
}
static void *safeP(void *p) {
    if (p == NULL)
        fail();
    return p;
}

void show(display *d) {
    SDL_RenderPresent(d->renderer);
    SDL_Delay(10);
}

int getWidth(display *d) { return d->width; }

int getHeight(display *d) { return d->height; }

void line(display *d, int x0, int y0, int x1, int y1) {
    safeI(SDL_RenderDrawLine(d->renderer, x0, y0, x1, y1));
}

void block(display *d, int x, int y, int w, int h) {
    SDL_Rect r = (SDL_Rect){x, y, w, h};
    safeI(SDL_RenderFillRect(d->renderer, &r));
}

void pixel(display *d, int x, int y) { safeI(SDL_RenderDrawPoint(d->renderer, x, y)); }

void colour(display *d, int rgba) {
    Uint8 r = (rgba >> 24) & 0xFF;
    Uint8 g = (rgba >> 16) & 0xFF;
    Uint8 b = (rgba >> 8) & 0xFF;
    Uint8 a = rgba & 0xFF;
    safeI(SDL_SetRenderDrawColor(d->renderer, r, g, b, a));
}

display *newDisplay(char *title, int width, int height) {
    setbuf(stdout, NULL);
    display *d = malloc(sizeof(display));
    safeI(SDL_Init(SDL_INIT_VIDEO));
    d->width = width;
    d->height = height;
    d->window = safeP(SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                       width, height, SDL_WINDOW_SHOWN));
    d->renderer = safeP(SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED));
    safeI(SDL_RenderClear(d->renderer));
    colour(d, 0xFFFFFFFF);
    show(d);
    return d;
}

void run(display *d, void *data, bool action(display *, void *, const char)) {
    bool quit = false;
    char key = 0;
    SDL_Event e;
    while (!quit) {
        quit = action(d, data, key);
        key = 0;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN)
                key = (char)e.key.keysym.sym;
            if (e.type == SDL_QUIT)
                quit = true;
        }
    }
}

void freeDisplay(display *d) {
    SDL_DestroyRenderer(d->renderer);
    SDL_DestroyWindow(d->window);
    SDL_Quit();
    free(d);
}
