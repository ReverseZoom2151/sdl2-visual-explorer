#include "action.h"
#include "base.h"
#include "direction.h"

#include <assert.h>
#include <stdio.h>

static void test_directions(void) {
    assert(deltaX(North) == 0 && deltaY(North) == -1);
    assert(deltaX(East) == 1 && deltaY(East) == 0);
    assert(deltaX(SouthWest) == -1 && deltaY(SouthWest) == 1);
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

    freeState(game);
    freeGrid(board);
}

int main(void) {
    test_directions();
    test_player_collects_star();
    puts("emerald core tests passed");
    return 0;
}
