#include <stdbool.h>

struct grid;
typedef struct grid grid;

struct entity;
typedef struct entity entity;
typedef int direction;

grid *newGrid(int width, int height);

void freeGrid(grid *g);

int gridWidth(const grid *g);
int gridHeight(const grid *g);
bool gridContains(const grid *g, int x, int y);

entity *getCell(grid *g, int x, int y);
entity *tryGetCell(grid *g, int x, int y);

void setCell(grid *g, int x, int y, entity *e);
entity *nextCell(grid *g, int x, int y, direction d);
