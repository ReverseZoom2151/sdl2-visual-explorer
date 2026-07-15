#include "emerald/action.h"
#include "emerald/direction.h"
#include "emerald/display.h"
#include "emerald/level.h"
#include <stdbool.h>

enum {
    tileSize = 64,
    tileInset = 4,
    gemCore = 16,
};

static const int width = 9, height = 9;
static const char *const level[] = {"#########", "#...#..*#", "#.#.#.###", "#.#....*#", "#....#.##",
                                    "#@#*....#", "####.##.#", "#*...#*.#", "#########"};

static void drawFloor(display *display, int left, int top) {
    colour(display, 0x0B1220FFU);
    block(display, left, top, tileSize, tileSize);
    colour(display, 0x172033FFU);
    block(display, left, top + tileSize - 1, tileSize, 1);
    block(display, left + tileSize - 1, top, 1, tileSize);
}

static void drawWall(display *display, int left, int top) {
    colour(display, 0x172554FFU);
    block(display, left, top, tileSize, tileSize);
    colour(display, 0x1D4ED8FFU);
    block(display, left + tileInset, top + tileInset, tileSize - 2 * tileInset,
          tileSize - 2 * tileInset);
    colour(display, 0x60A5FAFFU);
    block(display, left + tileInset, top + tileInset, tileSize - 2 * tileInset, tileInset);
}

static void drawGem(display *display, int left, int top) {
    drawFloor(display, left, top);
    colour(display, 0x10B981FFU);
    block(display, left + tileSize / 2 - gemCore / 2, top + tileSize / 2 - gemCore, gemCore,
          2 * gemCore);
    block(display, left + tileSize / 2 - gemCore, top + tileSize / 2 - gemCore / 2, 2 * gemCore,
          gemCore);
    colour(display, 0xA7F3D0FFU);
    block(display, left + tileSize / 2 - gemCore / 4, top + tileSize / 2 - gemCore / 2, gemCore / 2,
          gemCore / 2);
}

static void drawPlayer(display *display, int left, int top) {
    drawFloor(display, left, top);
    colour(display, 0x0EA5E9FFU);
    block(display, left + 16, top + 16, 32, 32);
    colour(display, 0xE0F2FEFFU);
    block(display, left + 23, top + 23, 18, 18);
}

static void draw(display *display, grid *board) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            entity *e = getCell(board, x, y);
            int left = x * tileSize;
            int top = y * tileSize;
            switch (getKind(e)) {
            case Blank:
                drawFloor(display, left, top);
                break;
            case Wall:
                drawWall(display, left, top);
                break;
            case Star:
                drawGem(display, left, top);
                break;
            case Player:
                drawPlayer(display, left, top);
                break;
            }
        }
    }
}

static void drawComplete(display *display) {
    colour(display, 0x064E3BFFU);
    block(display, 0, 0, tileSize * width, tileSize * height);
    colour(display, 0x34D399FFU);
    block(display, tileSize * 2, tileSize * 2, tileSize * 5, tileSize * 5);
    colour(display, 0xECFDF5FFU);
    block(display, tileSize * 3, tileSize * 3, tileSize * 3, tileSize * 3);
}

bool navigate(display *d, void *data, SDL_Keycode pressedKey) {
    entity *player = (entity *)data;
    grid *g = getGrid(player);
    int stars = getStars(getState(player));
    if (stars != 0) {
        if (pressedKey == SDLK_LEFT)
            act(player, West);
        else if (pressedKey == SDLK_RIGHT)
            act(player, East);
        else if (pressedKey == SDLK_UP)
            act(player, North);
        else if (pressedKey == SDLK_DOWN)
            act(player, South);
        draw(d, g);
    } else {
        drawComplete(d);
    }
    show(d);
    return pressedKey == SDLK_ESCAPE;
}

int main() {
    grid *g = newGrid(width, height);
    state *s = newState();
    if (g == NULL || s == NULL || !populateLevel(s, g, level)) {
        freeState(s);
        freeGrid(g);
        return 1;
    }
    entity *player = getPlayer(s);
    display *d = newDisplay("Emerald Maze", width * tileSize, height * tileSize);
    if (d == NULL) {
        freeState(s);
        freeGrid(g);
        return 1;
    }
    run(d, (void *)player, navigate);
    freeState(s);
    freeGrid(g);
    freeDisplay(d);
    return 0;
}
