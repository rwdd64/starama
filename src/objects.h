#ifndef OBJECTS_H 
#define OBJECTS_H 

#include "dtypes.h"

typedef enum {
    PLAYER,
    PROJECTILE,
} ObjectType;

typedef struct {
    ObjectType type;
    Vec2 position;
    float width;
    float height;
    float aimAngle;
} Player;

typedef struct {
    ObjectType type;
    Vec2 position;
    Vec2 velocity;
    float width;
    float height;
} Projectile;

typedef union {
    ObjectType type;
    Player player;
    Projectile projectile;
} Object;

typedef struct ObjectNode {
    Object *object;
    struct ObjectNode *prev;
    struct ObjectNode *next;
} ObjectNode;

#endif // OBJECTS_H 
