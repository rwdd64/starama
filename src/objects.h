#ifndef OBJECTS_H 
#define OBJECTS_H 

#include "dtypes.h"

typedef struct {
    Vec2 position;
    float width;
    float height;
    float aimAngle;
} Player;

typedef struct {
    Vec2 position;
    Vec2 velocity;
    float width;
    float height;
} Projectile;

#endif // OBJECTS_H 
