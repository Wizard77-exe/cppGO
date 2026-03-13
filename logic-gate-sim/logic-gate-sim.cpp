#include <SDL2/SDL.h>
#include <math.h>
#include <iostream>
#include <string>

using namespace std;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL Initialization Failed: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (!(initSDL())) return 1;

    SDL_Window *window = SDL_CreateWindow("Logic Gate Simulator",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600, 0
    );

    if (!window) {
        SDL_Log("Window Creation Failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        SDL_Log("Renderer Creation Failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    // Variables
    SDL_Event event;
    bool running = true;
    float x = 0.0f, y = 275.0f;
    int width = 50, height = 50;

    const int FPS = 60;
    float speed = 200.0f;

    Uint32 lastTime = SDL_GetTicks();                          // LAST FRAME TIME

    bool goingRight = true;
    // Main Loop: Checks if x is clicked and draw
    while(running) {
        Uint32 currentTime = SDL_GetTicks();                    // FRAME START TIME
        float dt = (currentTime - lastTime) / 1000.0f;          // DELTA TIME IN SECOND
        lastTime = currentTime;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // UPDATE POSITION
        if (goingRight) {
            x += speed * dt;
            if ((x + width) >= 800) goingRight = false;  
        } else {
            x -= speed * dt;
            if (x <= 0) goingRight = true;
        }

        // DRAW
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);         // BLACK BACKGROUND
        SDL_RenderClear(renderer);                              // CLEAR RENDERER

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // WHITE
        SDL_Rect rect = {(int)x, (int)y, width, height};                  // RECTANGLE OUTLINE
        SDL_RenderFillRect(renderer, &rect);                    // DRAWS A POINT IN THE MIDDLE OF THE WINDOW(800, 600)

        //SHOW THE OUTPUT
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}