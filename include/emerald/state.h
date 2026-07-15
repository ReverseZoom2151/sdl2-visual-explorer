#include <stdbool.h>

struct state;
typedef struct state state;

struct entity;
typedef struct entity entity;

state *newState();

void freeState(state *s);
void setPlayer(state *s, entity *p);

entity *getPlayer(state *s);

void addStar(state *s);

int getStars(state *s);

void findStar(state *s);

bool ended(state *s);
