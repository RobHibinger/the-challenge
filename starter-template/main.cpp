#define SDL_MAIN_USE_CALLBACKS 1

#include <iostream>
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

typedef struct AppState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_AppResult app_quit = SDL_APP_CONTINUE;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    SDL_Window* window = SDL_CreateWindow("Starter Template", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
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
        .renderer = renderer
    };

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState* as = (AppState*)appstate;

    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    if (keyboardState[SDL_SCANCODE_Q]) {
        as->app_quit = SDL_APP_SUCCESS;
    }

    SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(as->renderer);
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
