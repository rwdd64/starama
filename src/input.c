void handleKeydown(SDL_Keycode key) {
    switch (key) {
        case SDLK_Q:
            running = false;
            break;
        case SDLK_RIGHT:
            turnRight(10);
            break;
        case SDLK_LEFT:
            turnLeft(10);
            break;
        case SDLK_SPACE:
            shoot();
            break;
        case SDLK_S:
            saveState();
            break;
        case SDLK_L:
            loadState();
            break;
    }
}

void handleKeyup(SDL_Keycode key) {
    switch (key) {}
}
