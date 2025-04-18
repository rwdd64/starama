void updateProjectile() {
    if (state.projectileActive) {
        state.projectile.position.x += state.projectile.velocity.x*0.5;
        state.projectile.position.y += state.projectile.velocity.y*0.5;
        if (state.projectile.position.x >= WIDTH ||
            state.projectile.position.x < 0 ||
            state.projectile.position.y >= HEIGHT ||
            state.projectile.position.y < 0
            ) {
            state.projectile = (Projectile){0};
            state.projectileActive = false;
        }

    }
}

void renderProjectile(SDL_Renderer *renderer) {
    if (state.projectileActive) {
        SDL_FRect rect = {
            state.projectile.position.x-state.projectile.width/2,
            state.projectile.position.y-state.projectile.height/2,
            state.projectile.width,
            state.projectile.height,
        };
        SDL_RenderFillRect(renderer, &rect);
    }
}
