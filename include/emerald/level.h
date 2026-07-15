#pragma once

#include "emerald/grid.h"
#include "emerald/state.h"

#include <stdbool.h>

/**
 * Populate an empty grid from width-sized ASCII rows.
 *
 * The map accepts '.', '#', '*', and '@', and must contain exactly one player.
 * It is intentionally independent of SDL so maps can be validated in tests.
 */
bool populateLevel(state *game, grid *board, const char *const rows[]);
