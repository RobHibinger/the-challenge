#define SDL_MAIN_USE_CALLBACKS 1

#include <iostream>
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float GAME_SPEED = 250.0f;

struct Vec2f {
    float x = 0.0f, y = 0.0f;
};

struct GameObject {
    SDL_FRect rect = {0.0f, 0.0f, 0.0f, 0.0f};
    Vec2f vel;
};

struct GameState {
    GameObject left_paddle;
    GameObject right_paddle;
    GameObject ball;

    static GameState DefaultGameState() {
        float paddle_width = 20.0f;
        float paddle_height = 100.0f;
        float ball_width = 20.0f;
        float balll_height = 20.0f;
        
        return GameState {
            .left_paddle = GameObject {
                .rect = SDL_FRect {
                    .x = 0,
                    .y = WINDOW_HEIGHT / 2 - paddle_height / 2,
                    .w = paddle_width,
                    .h = paddle_height,
                },
            },
            .right_paddle = GameObject {
                .rect =  SDL_FRect {
                    .x = WINDOW_WIDTH - paddle_width,
                    .y = WINDOW_HEIGHT / 2 - paddle_height / 2,
                    .w = paddle_width,
                    .h = paddle_height,
                },
            },
            .ball = GameObject {
                .rect  = SDL_FRect {
                    .x = WINDOW_WIDTH / 2 - ball_width / 2,
                    .y = WINDOW_HEIGHT / 2 - balll_height / 2,
                    .w = ball_width,
                    .h = balll_height,
                },
                .vel = {1.0f, 1.0f},
            },
        };
    }
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
    as->game_state.left_paddle.vel.y = 0;
    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    if (keyboardState[SDL_SCANCODE_W]) {
        as->game_state.left_paddle.vel.y = -1.0f;
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        as->game_state.left_paddle.vel.y = 1.0f;
    }
}

bool is_colliding_aabb(const GameObject& o1, const GameObject& o2) {
    return o1.rect.x < o2.rect.x + o2.rect.w &&
        o1.rect.x + o1.rect.w > o2.rect.x &&
        o1.rect.y < o2.rect.y + o2.rect.h &&
        o1.rect.y + o1.rect.h > o2.rect.y;
}

void update_game(AppState* as) {
    float right_paddle_ball_diff = as->game_state.right_paddle.rect.y + as->game_state.right_paddle.rect.h / 2 - as->game_state.ball.rect.y + as->game_state.ball.rect.h / 2;
    as->game_state.right_paddle.vel.y = 0;
    if (right_paddle_ball_diff != 0) {
        as->game_state.right_paddle.vel.y = right_paddle_ball_diff > 0 ? -1.0f : 1.0f;
    }

    if (as->game_state.ball.rect.y <= 0 || as->game_state.ball.rect.y + as->game_state.ball.rect.h >= WINDOW_HEIGHT) {
        as->game_state.ball.vel.y = -1.0f * as->game_state.ball.vel.y;
    }

    if (is_colliding_aabb(as->game_state.right_paddle, as->game_state.ball) || is_colliding_aabb(as->game_state.left_paddle, as->game_state.ball)) {
        as->game_state.ball.vel.x = -1.0f * as->game_state.ball.vel.x;
    }

    if (as->game_state.ball.rect.x < 0 || as->game_state.ball.rect.x > WINDOW_WIDTH) {
        as->game_state = GameState::DefaultGameState();
    }

    as->game_state.left_paddle.rect.y += GAME_SPEED * as->delta_time * as->game_state.left_paddle.vel.y * 2.0f;
    as->game_state.right_paddle.rect.y += GAME_SPEED * as->delta_time * as->game_state.right_paddle.vel.y * 1.0f;
    as->game_state.ball.rect.x += GAME_SPEED * as->delta_time * as->game_state.ball.vel.x;
    as->game_state.ball.rect.y += GAME_SPEED * as->delta_time * as->game_state.ball.vel.y;

    SDL_SetRenderDrawColor(as->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(as->renderer, &as->game_state.left_paddle.rect);
    SDL_RenderFillRect(as->renderer, &as->game_state.right_paddle.rect);
    SDL_RenderFillRect(as->renderer, &as->game_state.ball.rect);
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

   *appstate = new AppState {
        .window = window,
        .renderer = renderer,
        .game_state = GameState::DefaultGameState(),
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
