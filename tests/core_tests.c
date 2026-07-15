#include "emerald/action.h"
#include "emerald/base.h"
#include "emerald/direction.h"
#include "emerald/fractal.h"
#include "emerald/level.h"

#include <assert.h>
#include <stdio.h>

static void test_directions(void) {
    assert(deltaX(North) == 0 && deltaY(North) == -1);
    assert(deltaX(East) == 1 && deltaY(East) == 0);
    assert(deltaX(SouthWest) == -1 && deltaY(SouthWest) == 1);
}

static void test_grid_rejects_invalid_dimensions_and_bounds(void) {
    assert(newGrid(0, 3) == NULL);
    assert(newGrid(3, 0) == NULL);

    grid *board = newGrid(2, 2);
    assert(board != NULL);
    assert(gridWidth(board) == 2 && gridHeight(board) == 2);
    assert(gridContains(board, 1, 1));
    assert(!gridContains(board, -1, 0));
    assert(tryGetCell(board, 2, 1) == NULL);
    freeGrid(board);
}

static void test_player_collects_star(void) {
    state *game = newState();
    grid *board = newGrid(3, 3);
    entity *cells[3][3];
    const char *layout[] = {"###", "#@*", "###"};

    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            cells[y][x] = newEntity(game, board, x, y, layout[y][x]);
            wake(cells[y][x]);
        }
    }

    entity *player = getPlayer(game);
    assert(player == cells[1][1]);
    assert(getStars(game) == 1);

    act(player, East);

    assert(getStars(game) == 0);
    assert(ended(game));
    assert(getKind(getCell(board, 2, 1)) == Player);
    assert(getKind(getCell(board, 1, 1)) == Blank);

    act(player, South);
    assert(getKind(getCell(board, 2, 1)) == Player);

    freeState(game);
    freeGrid(board);
}

static void test_level_validation(void) {
    state *game = newState();
    grid *board = newGrid(3, 2);
    const char *const valid[] = {"@.*", "###"};
    assert(populateLevel(game, board, valid));
    assert(getStars(game) == 1);
    assert(getKind(getPlayer(game)) == Player);
    freeState(game);
    freeGrid(board);

    const char *const missing_player[] = {"...", "###"};
    game = newState();
    board = newGrid(3, 2);
    assert(!populateLevel(game, board, missing_player));
    freeState(game);
    freeGrid(board);

    const char *const invalid_tile[] = {"@..", "#x#"};
    game = newState();
    board = newGrid(3, 2);
    assert(!populateLevel(game, board, invalid_tile));
    freeState(game);
    freeGrid(board);
}

static void test_fractal_math(void) {
    fractalView view = defaultFractalView();
    assert(mandelbrotIterations(&view, 50, 50, 100, 100, 64) == 64);
    assert(mandelbrotIterations(&view, 99, 99, 100, 100, 64) < 64);
    assert(mandelbrotIterations(&view, -1, 0, 100, 100, 64) == 0);
    assert(zoomFractal(&view, 0.5));
    assert(view.scale == 0.75);
    assert(panFractal(&view, 0.1, -0.2));
    assert(!zoomFractal(&view, 0.0));
}

int main(void) {
    test_directions();
    test_grid_rejects_invalid_dimensions_and_bounds();
    test_player_collects_star();
    test_level_validation();
    test_fractal_math();
    puts("SDL2 Visual Explorer core tests passed");
    return 0;
}
