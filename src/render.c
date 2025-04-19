void rotatePoint(SDL_Point pivot, SDL_Point *point, float angle) {
    float angleRad = angle*M_PI/180.0f;

    float cosine = cosf(angleRad);
    float sine = sinf(angleRad);

    point->x -= pivot.x;
    point->y -= pivot.y;

    float new_x = point->x * cosine - point->y * sine;
    float new_y = point->x * sine + point->y * cosine;

    point->x = new_x;
    point->y = new_y;

    point->x += pivot.x;
    point->y += pivot.y;
}

void renderTriangle(SDL_Renderer *renderer, SDL_Point origin, float w, float h, float angle) {
    SDL_Point a = {0, -(2*h/3)};
    SDL_Point b = {-w/2, (h/3)};
    SDL_Point c = {w/2, (h/3)};

    SDL_Point center = {
        (a.x+b.x+c.x)/3,
        (a.y+b.y+c.y)/3
    };

    rotatePoint(center, &a, angle);
    a.x += origin.x;
    a.y += origin.y;

    rotatePoint(center, &b, angle);
    b.x += origin.x;
    b.y += origin.y;

    rotatePoint(center, &c, angle);
    c.x += origin.x;
    c.y += origin.y;

    SDL_RenderLine(renderer, a.x, a.y, b.x, b.y);
    SDL_RenderLine(renderer, b.x, b.y, c.x, c.y);
    SDL_RenderLine(renderer, c.x, c.y, a.x, a.y);
}

void render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    SDL_Point player_position = {
        state.player.position.x,
        state.player.position.y,
    };

    renderTriangle(
        renderer,
        player_position,
        state.player.width,
        state.player.height,
        state.player.aimAngle
    );

    objectHeadRewind();

    while (state.objectHead != NULL) {
        switch (state.objectHead->object->type) {
            case PLAYER:
                break;
            case PROJECTILE:
                renderProjectile(renderer, state.objectHead);
                break;
        }

        if (state.objectHead->next != NULL) {
            state.objectHead = state.objectHead->next;
        } else {
            break;
        }
    }

    SDL_RenderPresent(renderer);
}
