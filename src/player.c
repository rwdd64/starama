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
    float angleRad = (state.player.aimAngle+90)*M_PI/180.0f;

    state.projectile = (Projectile){
        .height = 25,
        .width = 25,
        .velocity = (Vec2){-cosf(angleRad), -sinf(angleRad)},
        .position = player_position,
    };
}
