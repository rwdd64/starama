#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "dtypes.h"
#include "objects.h"

int WIDTH = 800;
int HEIGHT = 600;

typedef struct {
    Player player;
    ObjectNode *objectHead;
} GameState;

static GameState state;
static bool running;

#include "state.c"
#include "objlist.c"
#include "player.c"
#include "projectile.c"
#include "input.c"
#include "render.c"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    state = (GameState){0};
    state.player.position = (Vec2){
        state.player.width/2+WIDTH/2,
        state.player.height/2+HEIGHT/2
    };
    state.player.width = 60;
    state.player.height = 75;
    state.player.aimAngle = 270;

    state.objectHead = NULL;

    running = true;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow(
            "starama",
            WIDTH,
            HEIGHT,
            SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
    if (window == NULL) {
        SDL_Log("Couldn't initialize Window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        SDL_Log("Couldn't initialize Renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    objectHeadRewind();

    while (state.objectHead != NULL) {
        switch (state.objectHead->object->type) {
            case PLAYER:
                break;
            case PROJECTILE:
                updateProjectile(&state.objectHead);
                break;
        }

        if (state.objectHead != NULL && state.objectHead->next != NULL) {
            state.objectHead = state.objectHead->next;
        } else {
            break;
        }
    }

    if (!running) {
        return SDL_APP_SUCCESS;
    }

    render(renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *ev) {
    switch (ev->type) {
        case SDL_EVENT_QUIT:
            running = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            handleKeydown(ev->key.key);
            break;
        case SDL_EVENT_KEY_UP:
            handleKeyup(ev->key.key);
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            state.player.width += ev->wheel.y * 20;
            state.player.height += ev->wheel.y * 20;
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            SDL_GetWindowSize(window, &WIDTH, &HEIGHT);
            break;
    }

    if (!running) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}
