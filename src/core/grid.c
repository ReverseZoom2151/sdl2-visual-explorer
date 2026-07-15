#include "emerald/grid.h"

#include "emerald/base.h"
#include "emerald/direction.h"
#include "emerald/entity.h"

#include <stdint.h>
#include <stdlib.h>

struct grid {
    int width;
    int height;
    entity ***cells;
};

static void checkBounds(const grid *board, int x, int y) {
    if (!gridContains(board, x, y))
        fail("grid coordinates out of bounds");
}

grid *newGrid(int width, int height) {
    if (width <= 0 || height <= 0)
        return NULL;
    if ((size_t)width > SIZE_MAX / sizeof(entity **))
        return NULL;

    grid *board = calloc(1, sizeof(*board));
    if (board == NULL)
        return NULL;
    board->width = width;
    board->height = height;
    board->cells = calloc((size_t)width, sizeof(*board->cells));
    if (board->cells == NULL) {
        free(board);
        return NULL;
    }

    for (int x = 0; x < width; ++x) {
        if ((size_t)height > SIZE_MAX / sizeof(*board->cells[x])) {
            freeGrid(board);
            return NULL;
        }
        board->cells[x] = calloc((size_t)height, sizeof(*board->cells[x]));
        if (board->cells[x] == NULL) {
            freeGrid(board);
            return NULL;
        }
    }
    return board;
}

void freeGrid(grid *board) {
    if (board == NULL)
        return;
    for (int x = 0; x < board->width; ++x) {
        for (int y = 0; y < board->height; ++y)
            freeEntity(board->cells[x][y]);
        free(board->cells[x]);
    }
    free(board->cells);
    free(board);
}

int gridWidth(const grid *board) { return board == NULL ? 0 : board->width; }

int gridHeight(const grid *board) { return board == NULL ? 0 : board->height; }

bool gridContains(const grid *board, int x, int y) {
    return board != NULL && x >= 0 && x < board->width && y >= 0 && y < board->height;
}

entity *getCell(grid *board, int x, int y) {
    checkBounds(board, x, y);
    return board->cells[x][y];
}

entity *tryGetCell(grid *board, int x, int y) {
    return gridContains(board, x, y) ? board->cells[x][y] : NULL;
}

void setCell(grid *board, int x, int y, entity *value) {
    checkBounds(board, x, y);
    board->cells[x][y] = value;
}

entity *nextCell(grid *board, int x, int y, direction value) {
    return tryGetCell(board, x + deltaX(value), y + deltaY(value));
}
