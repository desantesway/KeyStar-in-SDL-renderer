
#include "main.hpp"

#define SDL_FLAGS SDL_INIT_VIDEO

bool SDL_Init(SDL* sdl) {

    sdl->SetRunning(true);
    CHECK_RESULT(sdl->Init(), "Error initializing SDL:");
    CHECK_RESULT(sdl->LoadInitMedia(), "Error loading media:");

    SDL_SetRenderDrawColor(sdl->GetRenderer(), 54, 54, 54, SDL_ALPHA_OPAQUE_FLOAT);

    return true;  /* carry on with the program! */
}

void SDL_Free(SDL* sdl) {
    if (sdl) {
		delete sdl;
    }
}

void SDL_Run(SDL* sdl) {
    while (sdl->IsRunning()) {
		sdl->GameEvents();  // Call the game events function

        sdl->ClearFrame();

        sdl->UpdateScreen();

        sdl->RenderInit();

        sdl->RenderFrame();
    }
    VOID_CHECK_RESULT(!sdl->FreeAllMedia(), "Error freeing media:");
}

int main(int argc, char* args[]) {

    SDL_SetMainReady(); 

    SDL* engine = new SDL();

    if (SDL_Init(engine)) {
        SDL_Run(engine);
    }

    SDL_Free(engine);

    return true;
}

