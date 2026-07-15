#include "emerald/state.h"
#include <stdlib.h>

struct state {
    entity *player;
    int stars;
};

state *newState() {
    state *s = malloc(sizeof(state));
    if (s == NULL)
        return NULL;
    *s = (struct state){NULL, 0};
    return s;
}

void freeState(state *s) { free(s); }

void setPlayer(state *s, entity *p) { s->player = p; }

entity *getPlayer(state *s) { return s->player; }

void addStar(state *s) { s->stars++; }

int getStars(state *s) { return s->stars; }

void findStar(state *s) {
    if (s->stars > 0)
        --s->stars;
}

bool ended(state *s) { return s == NULL || s->stars == 0; }
