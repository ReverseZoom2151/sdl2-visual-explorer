#include "emerald/level.h"

#include "emerald/action.h"
#include "emerald/entity.h"

#include <string.h>

static bool isTile(char tile) {
    return tile == Blank || tile == Wall || tile == Star || tile == Player;
}

bool populateLevel(state *game, grid *board, const char *const rows[]) {
    if (game == NULL || board == NULL || rows == NULL)
        return false;

    int players = 0;
    for (int y = 0; y < gridHeight(board); ++y) {
        if (rows[y] == NULL || strlen(rows[y]) != (size_t)gridWidth(board))
            return false;
        for (int x = 0; x < gridWidth(board); ++x) {
            if (!isTile(rows[y][x]) || getCell(board, x, y) != NULL)
                return false;
            if (rows[y][x] == Player)
                ++players;
        }
    }
    if (players != 1)
        return false;

    for (int y = 0; y < gridHeight(board); ++y) {
        for (int x = 0; x < gridWidth(board); ++x) {
            entity *tile = newEntity(game, board, x, y, rows[y][x]);
            if (tile == NULL)
                return false;
            wake(tile);
        }
    }
    return true;
}
