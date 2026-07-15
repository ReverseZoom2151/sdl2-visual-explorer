#include "emerald/display.h"
#include "emerald/fractal.h"

#include <stdbool.h>
#include <stdlib.h>

enum {
    pixelStep = 2,
    maximumIterations = 128,
    windowSize = 640,
};

typedef struct explorerState {
    fractalView view;
    bool needsRender;
} explorerState;

static Uint32 palette(int iteration) {
    if (iteration >= maximumIterations)
        return 0x07111FFF;
    Uint8 red = (Uint8)((iteration * 9) % 256);
    Uint8 green = (Uint8)((iteration * 5 + 40) % 256);
    Uint8 blue = (Uint8)((iteration * 13 + 90) % 256);
    return ((Uint32)red << 24) | ((Uint32)green << 16) | ((Uint32)blue << 8) | 0xFFU;
}

static void drawFractal(display *display, const fractalView *view) {
    int width = getWidth(display);
    int height = getHeight(display);
    for (int y = 0; y < height; y += pixelStep) {
        for (int x = 0; x < width; x += pixelStep) {
            int iteration = mandelbrotIterations(view, x, y, width, height, maximumIterations);
            colour(display, palette(iteration));
            block(display, x, y, pixelStep, pixelStep);
        }
    }
    show(display);
}

static bool handleInput(explorerState *state, SDL_Keycode key) {
    bool changed = false;
    switch (key) {
    case SDLK_z:
        changed = zoomFractal(&state->view, 0.75);
        break;
    case SDLK_x:
        changed = zoomFractal(&state->view, 1.0 / 0.75);
        break;
    case SDLK_LEFT:
        changed = panFractal(&state->view, -0.15, 0.0);
        break;
    case SDLK_RIGHT:
        changed = panFractal(&state->view, 0.15, 0.0);
        break;
    case SDLK_UP:
        changed = panFractal(&state->view, 0.0, -0.15);
        break;
    case SDLK_DOWN:
        changed = panFractal(&state->view, 0.0, 0.15);
        break;
    default:
        break;
    }
    state->needsRender = state->needsRender || changed;
    return key == SDLK_ESCAPE;
}

static bool navigateMandelbrot(display *display, void *data, SDL_Keycode key) {
    explorerState *state = data;
    if (state == NULL)
        return true;
    bool quit = handleInput(state, key);
    if (state->needsRender) {
        drawFractal(display, &state->view);
        state->needsRender = false;
    }
    return quit;
}

int main(void) {
    display *display = newDisplay("Emerald Fractal Explorer", windowSize, windowSize);
    if (display == NULL)
        return EXIT_FAILURE;
    explorerState state = {defaultFractalView(), true};
    run(display, &state, navigateMandelbrot);
    freeDisplay(display);
    return EXIT_SUCCESS;
}
