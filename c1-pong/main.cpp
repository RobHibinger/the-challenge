#define SDL_MAIN_USE_CALLBACKS 1

#include <iostream>
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float GAME_SPEED = 500.0f;

struct GameState {
    SDL_FRect left_paddle;
    SDL_FRect right_paddle;
    SDL_FRect ball;
};

struct AppState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    GameState game_state;
    SDL_AppResult app_quit = SDL_APP_CONTINUE;
    Uint64 previous_ticks = SDL_GetTicksNS();
    Uint64 current_ticks = 0;
    float delta_time = 0;
};

void process_input(AppState* as) {
    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    if (keyboardState[SDL_SCANCODE_W]) {
        as->game_state.left_paddle.y -= 1.0f;
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        as->game_state.left_paddle.y += 1.0f;
    }
}

void update_game(AppState* as) {
    as->game_state.ball.x += GAME_SPEED * as->delta_time;

    

    SDL_SetRenderDrawColor(as->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(as->renderer, &as->game_state.left_paddle);
    SDL_RenderFillRect(as->renderer, &as->game_state.right_paddle);
    SDL_RenderFillRect(as->renderer, &as->game_state.ball);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    SDL_Window* window = SDL_CreateWindow("Pong", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (!window){
        return SDL_APP_FAILURE;
    }
    
    // create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer){
        return SDL_APP_FAILURE;
    }

    float paddle_width = 20.0f;
    float paddle_height = 100.0f;
    float ball_width = 20.0f;
    float balll_height = 20.0f;

   *appstate = new AppState {
        .window = window,
        .renderer = renderer,
        .game_state = GameState {
            .left_paddle = SDL_FRect {
                .x = 0,
                .y = WINDOW_HEIGHT / 2 - paddle_height / 2,
                .w = paddle_width,
                .h = paddle_height,
            },
            .right_paddle = SDL_FRect {
                .x = WINDOW_WIDTH - paddle_width,
                .y = WINDOW_HEIGHT / 2 - paddle_height / 2,
                .w = paddle_width,
                .h = paddle_height,
            },
            .ball = SDL_FRect {
                .x = WINDOW_WIDTH / 2 - ball_width / 2,
                .y = WINDOW_HEIGHT / 2 - balll_height / 2,
                .w = ball_width,
                .h = balll_height,
            }
        }
    };

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState* as = (AppState*)appstate;

    as->current_ticks = SDL_GetTicksNS();
    as->delta_time = (as->current_ticks - as->previous_ticks) / 1e9f;
    as->previous_ticks = as->current_ticks;

    process_input(as);

    SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
    SDL_RenderClear(as->renderer);

    update_game(as);

    SDL_RenderPresent(as->renderer);
    SDL_Delay(1);

    return as->app_quit;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState* app = (AppState*)appstate;
    
    if (event->type == SDL_EVENT_QUIT) {
        app->app_quit = SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState* as = (AppState*)appstate;
    SDL_DestroyRenderer(as->renderer);
    SDL_DestroyWindow(as->window);
    delete as;
}
