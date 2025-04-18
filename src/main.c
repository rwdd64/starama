#include <stdio.h>
#include <math.h>
#include <SDL.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct {
    double x, y;
} Vec2;

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

typedef struct {
    Player player;
    Projectile projectile;
    bool projectileActive;
} GameState;

GameState state;
bool running;

void turnRight(float angle) {
    state.player.aimAngle = (int)(state.player.aimAngle+angle) % 360;
}

void turnLeft(float angle) {
    state.player.aimAngle = (int)(state.player.aimAngle-angle+360) % 360;
}

void shoot() {
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

void handleKeydown(SDL_Keycode key) {
    switch (key) {
        case SDLK_Q:
            running = false;
            break;
        case SDLK_D:
            turnRight(10);
            break;
        case SDLK_A:
            turnLeft(10);
            break;
        case SDLK_E:
            shoot();
            break;
    }
}

void handleKeyup(SDL_Keycode key) {
    switch (key) {}
}

float rotatePoint(SDL_Point pivot, SDL_Point *point, float angle) {
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

    if (state.projectileActive) {
        SDL_FRect rect = {
            state.projectile.position.x,
            state.projectile.position.y,
            state.projectile.width,
            state.projectile.height,
        };
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

int main(void) {
    state = (GameState){0};
    state.player.position = (Vec2){
        state.player.width/2+WIDTH/2,
        state.player.height/2+HEIGHT/2
    };
    state.player.width = 60;
    state.player.height = 75;
    running = true;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
            "starama",
            WIDTH,
            HEIGHT,
            SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    while (running) {

        SDL_Event ev;

        if (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    handleKeydown(ev.key.key);
                    break;
                case SDL_EVENT_KEY_UP:
                    handleKeyup(ev.key.key);
                    break;
                case SDL_EVENT_MOUSE_WHEEL:
                    state.player.width += ev.wheel.y * 20;
                    state.player.height += ev.wheel.y * 20;
                    break;
            }
        }

        if (state.projectileActive) {
            state.projectile.position.x += state.projectile.velocity.x;
            state.projectile.position.y += state.projectile.velocity.y;
        }

        render(renderer);


        SDL_Log("Player angle: %f", state.player.aimAngle);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
