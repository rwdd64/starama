void updateProjectile(ObjectNode **node) {
    Projectile *proj = &(*node)->object->projectile;

    proj->position.x += proj->velocity.x*0.5;
    proj->position.y += proj->velocity.y*0.5;

    if (proj->position.x >= WIDTH ||
        proj->position.x < 0 ||
        proj->position.y >= HEIGHT ||
        proj->position.y < 0
        ) {
        deleteNode(node);
    }
}

void renderProjectile(SDL_Renderer *renderer, ObjectNode *node) {
    Projectile *proj = &node->object->projectile;

    SDL_FRect rect = {
        proj->position.x-proj->width/2,
        proj->position.y-proj->height/2,
        proj->width,
        proj->height,
    };

    SDL_RenderFillRect(renderer, &rect);
}
