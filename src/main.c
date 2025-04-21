#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>

#include "dtypes.h"
#include "objects.h"

#define WIDTH 800
#define HEIGHT 600

typedef struct {
    Player player;
    ObjectNode *objectHead;
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

void deleteNode(ObjectNode **node) {
    ObjectNode *replacement = NULL;

    if ((*node)->prev != NULL && (*node)->next != NULL) {
        (*node)->prev->next = (*node)->next;
        (*node)->next->prev = (*node)->prev;
        replacement = (*node)->next;
    } else if ((*node)->prev != NULL) {
        (*node)->prev->next = NULL;
        replacement = (*node)->prev;
    } else if ((*node)->next != NULL) {
        (*node)->next->prev = NULL;
        replacement = (*node)->next;
    }

    free((*node));
    (*node) = replacement;
}

void objectHeadRewind() {
    if (state.objectHead == NULL) return;

    while (state.objectHead->prev != NULL) {
        state.objectHead = state.objectHead->prev;
    }
}

void objectHeadForward() {
    if (state.objectHead == NULL) return;

    while (state.objectHead->next != NULL) {
        state.objectHead = state.objectHead->next;
    }
}

#include "player.c"
#include "projectile.c"
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

    state.objectHead = NULL;

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

        render(renderer);

        SDL_Log("Player angle: %f", state.player.aimAngle);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
