#include <stdbool.h>
#include <SDL2/SDL.h>

// INITIALIZING SDL
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if(!(initSDL())) return 1;

    // INITIALIZING A WINDOW
    SDL_Window *window = SDL_CreateWindow(
        "Title Here",                               // TITLE OF THE WINDOW
        SDL_WINDOWPOS_CENTERED,                     // X POSITION
        SDL_WINDOWPOS_CENTERED,                     // Y POSITION
        800, 600, 0                                 // WIDTH, HEIGHT, FLAG 0 = DEFAULT    
    );

    // CHECKING IF WINDOW CREATION IS SUCCESSFUL, EXITING IF ERROR
    if (!window) {
        SDL_Log("Error making a window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // CREATING A RENDERER
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // CHECKING IF CREATING A RENDERER IS SUCCESSFUL
    if (!renderer) {
        SDL_Log("Renderer Failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // SDL EVENT
    SDL_Event event;
    bool running = true;
    float x = 0.0f, y = 275.0f;
    int width = 50, height = 50;

    const int FPS = 60;
    float speed = 200.0f;

    Uint32 lastTime = SDL_GetTicks();                          // LAST FRAME TIME

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
        x += speed * dt;
        if ((x + width) >= 800) x=0;                            // WRAP AROUND

        // DRAW
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);         // BLACK BACKGROUND
        SDL_RenderClear(renderer);                              // CLEAR RENDERER

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // WHITE
        SDL_Rect rect = {x, y, width, height};                  // RECTANGLE OUTLINE
        SDL_RenderFillRect(renderer, &rect);                    // DRAWS A POINT IN THE MIDDLE OF THE WINDOW(800, 600)

        //SHOW THE OUTPUT
        SDL_RenderPresent(renderer);
    }

    // CLEAN EVERYTHING
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}