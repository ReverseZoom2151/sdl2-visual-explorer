#include "emerald/grid.h"
#include "emerald/state.h"

struct entity;
typedef struct entity entity;

typedef char kind;

entity *newEntity(state *s, grid *g, int x, int y, kind k);

void freeEntity(entity *e);

grid *getGrid(entity *e);

kind getKind(entity *e);

state *getState(entity *e);

entity *getNext(entity *entity, direction d);

void move(entity *e, entity *target);
void mutate(entity *e, kind newKind);
