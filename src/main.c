#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL.h>

#include "dtypes.h"
#include "objects.h"

#define WIDTH 800
#define HEIGHT 600

typedef struct {
    Player player;
    Projectile projectile;
    bool projectileActive;
} GameState;

static GameState state;
static bool running;

bool saveState(void) {
    FILE *f = fopen(".state", "wb");
    if (f == NULL) {
        return false;
    }

    fwrite(&state, 1, sizeof(GameState), f);

    fclose(f);
    return true;
}

bool loadState(void) {
    FILE *f = fopen(".state", "rb");
    if (f == NULL) {
        return false;
    }

    fread(&state, 1, sizeof(GameState), f);
    fclose(f);
    return true;
}

#include "player.c"
#include "input.c"
#include "render.c"

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
