void turnRight(float angle) {
    state.player.aimAngle = (int)(state.player.aimAngle+angle) % 360;
}

void turnLeft(float angle) {
    state.player.aimAngle = (int)(state.player.aimAngle-angle+360) % 360;
}

void shoot(void) {
    Vec2 player_position = state.player.position;
    float angleRad = state.player.aimAngle*M_PI/180.0f;

    objectHeadForward();

    ObjectNode *nextNode = NULL;

    if (state.objectHead == NULL) {
        state.objectHead = malloc(sizeof(ObjectNode));
        nextNode = state.objectHead;
        nextNode->next = NULL;
        nextNode->prev = NULL;
    } else {
        state.objectHead->next = malloc(sizeof(ObjectNode));
        nextNode = state.objectHead->next;
        nextNode->next = NULL;
        nextNode->prev = state.objectHead;
    }

    nextNode->object = malloc(sizeof(Object));

    Object *nextObject = nextNode->object;

    nextObject->type = PROJECTILE;
    nextObject->projectile.height = 25;
    nextObject->projectile.width = 25;
    nextObject->projectile.velocity = (Vec2){
        cosf(angleRad), sinf(angleRad)
    };
    nextObject->projectile.position = player_position;
}
