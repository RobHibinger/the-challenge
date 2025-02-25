#define SDL_MAIN_USE_CALLBACKS 1

#include <iostream>
#include "glad/glad.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

typedef struct AppState {
    SDL_Window* window;
    SDL_GLContext glContext;
    SDL_AppResult app_quit = SDL_APP_CONTINUE;
    int window_width = WINDOW_WIDTH, window_height = WINDOW_HEIGHT;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    SDL_Window* window = SDL_CreateWindow("Learn OpenGL", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window){
        return SDL_APP_FAILURE;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cout << "Failed to create GL Context";
        return SDL_APP_FAILURE;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Failed to load glad";
        return SDL_APP_FAILURE;
    }
    
    glViewport(0, 0, 800, 600);
    *appstate = new AppState {
        .window = window,
        .glContext = glContext,
    };

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState* as = (AppState*)appstate;

    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    if (keyboardState[SDL_SCANCODE_Q]) {
        as->app_quit = SDL_APP_SUCCESS;
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(as->window);

    return as->app_quit;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState* app = (AppState*)appstate;
    
    if (event->type == SDL_EVENT_QUIT) {
        app->app_quit = SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        SDL_GetWindowSize(app->window, &app->window_width, &app->window_height);
        glViewport(0, 0, app->window_width, app->window_height);
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState* as = (AppState*)appstate;
    SDL_GL_DestroyContext(as->glContext);
    SDL_DestroyWindow(as->window);
    delete as;
}
