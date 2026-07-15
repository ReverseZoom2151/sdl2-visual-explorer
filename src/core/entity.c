#include "emerald/entity.h"
#include <stdlib.h>

struct entity {
    kind k;
    int x, y;
    state *s;
    grid *g;
};

entity *newEntity(state *s, grid *g, int x, int y, kind k) {
    if (s == NULL || g == NULL || !gridContains(g, x, y) || getCell(g, x, y) != NULL)
        return NULL;
    entity *e = malloc(sizeof(entity));
    if (e == NULL)
        return NULL;
    *e = (entity){k, x, y, s, g};
    setCell(g, x, y, e);
    return e;
}

void freeEntity(entity *e) { free(e); }

kind getKind(entity *e) { return e->k; }

state *getState(entity *e) { return e->s; }

grid *getGrid(entity *e) { return e->g; }

entity *getNext(entity *e, direction d) { return e == NULL ? NULL : nextCell(e->g, e->x, e->y, d); }

void move(entity *e, entity *target) {
    if (e == NULL || target == NULL || e->g != target->g)
        return;
    grid *g = e->g;
    int x = e->x;
    int y = e->y;
    e->x = target->x;
    e->y = target->y;
    target->x = x;
    target->y = y;
    setCell(g, target->x, target->y, target);
    setCell(g, e->x, e->y, e);
}

void mutate(entity *e, kind newKind) { e->k = newKind; }
