#include "display.h"

struct display {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width;
    int height;
};

static void reportSdlError(const char *context) {
    fprintf(stderr, "%s: %s\n", context, SDL_GetError());
}

static bool setRendererColour(SDL_Renderer *renderer, Uint32 rgba) {
    Uint8 red = (Uint8)((rgba >> 24) & 0xFFU);
    Uint8 green = (Uint8)((rgba >> 16) & 0xFFU);
    Uint8 blue = (Uint8)((rgba >> 8) & 0xFFU);
    Uint8 alpha = (Uint8)(rgba & 0xFFU);
    return SDL_SetRenderDrawColor(renderer, red, green, blue, alpha) == 0;
}

display *newDisplay(const char *title, int width, int height) {
    if (title == NULL || width <= 0 || height <= 0)
        return NULL;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        reportSdlError("SDL video initialization failed");
        return NULL;
    }

    display *value = calloc(1, sizeof(*value));
    if (value == NULL) {
        SDL_Quit();
        return NULL;
    }
    value->width = width;
    value->height = height;
    value->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width,
                                     height, SDL_WINDOW_SHOWN);
    if (value->window == NULL) {
        reportSdlError("SDL window creation failed");
        freeDisplay(value);
        return NULL;
    }
    value->renderer = SDL_CreateRenderer(value->window, -1, SDL_RENDERER_ACCELERATED);
    if (value->renderer == NULL)
        value->renderer = SDL_CreateRenderer(value->window, -1, SDL_RENDERER_SOFTWARE);
    if (value->renderer == NULL) {
        reportSdlError("SDL renderer creation failed");
        freeDisplay(value);
        return NULL;
    }
    colour(value, 0x000000FFU);
    SDL_RenderClear(value->renderer);
    return value;
}

void show(display *value) {
    if (value != NULL)
        SDL_RenderPresent(value->renderer);
}

int getWidth(display *value) { return value == NULL ? 0 : value->width; }

int getHeight(display *value) { return value == NULL ? 0 : value->height; }

void line(display *value, int x0, int y0, int x1, int y1) {
    if (value != NULL)
        SDL_RenderDrawLine(value->renderer, x0, y0, x1, y1);
}

void block(display *value, int x, int y, int width, int height) {
    if (value == NULL)
        return;
    SDL_Rect rectangle = {x, y, width, height};
    SDL_RenderFillRect(value->renderer, &rectangle);
}

void pixel(display *value, int x, int y) {
    if (value != NULL)
        SDL_RenderDrawPoint(value->renderer, x, y);
}

void colour(display *value, Uint32 rgba) {
    if (value != NULL && !setRendererColour(value->renderer, rgba))
        reportSdlError("SDL colour update failed");
}

void run(display *value, void *data, displayAction action) {
    if (value == NULL || action == NULL)
        return;

    bool quit = false;
    SDL_Keycode key = SDLK_UNKNOWN;
    while (!quit) {
        quit = action(value, data, key);
        key = SDLK_UNKNOWN;

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
                quit = true;
            else if (event.type == SDL_KEYDOWN)
                key = event.key.keysym.sym;
        }
        SDL_Delay(10);
    }
}

void freeDisplay(display *value) {
    if (value == NULL)
        return;
    if (value->renderer != NULL)
        SDL_DestroyRenderer(value->renderer);
    if (value->window != NULL)
        SDL_DestroyWindow(value->window);
    free(value);
    SDL_Quit();
}
