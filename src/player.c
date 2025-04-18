void turnRight(float angle) {
    state.player.aimAngle = (int)(state.player.aimAngle+angle) % 360;
}

void turnLeft(float angle) {
    state.player.aimAngle = (int)(state.player.aimAngle-angle+360) % 360;
}

void shoot(void) {
    if (state.projectileActive) return;
    state.projectileActive = true;

    Vec2 player_position = state.player.position;

    state.projectile = (Projectile){
        .height = 25,
        .width = 25,
        .velocity = (Vec2){0, -1},
        .position = player_position,
    };
}
