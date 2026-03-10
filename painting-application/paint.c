#include <SDL2/SDL.h>
#include <stdbool.h>

bool initSDL() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL Initialization Failed: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void draw(SDL_Renderer *renderer, int x, int y, int brushSize, SDL_Color color) {
    if (brushSize <= 0) brushSize = 1;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x - brushSize / 2, y - brushSize / 2, brushSize, brushSize};
    SDL_RenderFillRect(renderer, &rect);
}

void drawThickLine(SDL_Renderer *renderer, int lastX, int lastY, int currX, int currY, int brushSize, SDL_Color color) {
    float dx = currX - lastX;
    float dy = currY - lastY;
    float distance = sqrt(dx*dx + dy*dy);

    if (distance == 0) {
        draw(renderer, currX, currY, brushSize, color);
        return;
    }

    for (float i = 0; i < distance; i++) {
        float t = i / distance;
        int x = lastX + t*dx;
        int y = lastY + t*dy;
        draw(renderer, x, y, brushSize, color);
    }
}

int main(int argc, char *argv[]) {
    if(!(initSDL())) return 1;

    SDL_Window *window = SDL_CreateWindow(
        "Painting Application",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, 0
    );

    if (!window) {
        SDL_Log("Window Creation Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    if(!renderer) {
        SDL_Log("Renderer Creation Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    bool running = true;
    bool drawing = false;

    int brushSize = 5;

    int lastX, lastY;
    SDL_Color color = {255, 255, 255, 255};
    SDL_Event event;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //MAIN LOOP
    while(running) {
        frameStart = SDL_GetTicks();

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            } else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                drawing = true;
                lastX = event.button.x;
                lastY = event.button.y;
                drawThickLine(renderer, lastX, lastY, event.motion.x, event.motion.y, brushSize, color);
            } else if(event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT)drawing = false;
            } else if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_DOWN:
                        if(brushSize > 0) brushSize--;
                        break;
                    case SDLK_UP:
                        if(brushSize < 500) brushSize++;
                        break;
                    case SDLK_g:
                        color = (SDL_Color){0, 255, 0, 255};
                        break;
                    case SDLK_b:
                        color = (SDL_Color){0, 0, 255, 255};
                        break;
                    case SDLK_r:
                        color = (SDL_Color){255, 0, 0, 255};
                        break;
                    case SDLK_w:
                        color = (SDL_Color){255, 255, 255, 255};
                        break;
                    case SDLK_e:
                        color = (SDL_Color){0, 0, 0, 255};
                        break;
                    case SDLK_c:
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);
                        break;
                }
            }

            if (event.type == SDL_MOUSEMOTION && drawing) {
                drawThickLine(renderer, lastX, lastY, event.motion.x, event.motion.y, brushSize, color);
                lastX = event.motion.x;
                lastY = event.motion.y;
            }
        }

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}