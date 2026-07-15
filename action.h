#include "entity.h"

enum kind { Blank = '.', Wall = '#', Star = '*', Player = '@' };
typedef char kind;

void wake(entity *entity);
void die(entity *entity);
void act(entity *entity, direction d);
