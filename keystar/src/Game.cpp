/* TODOS:
* PRIORITY IN TEXTURE RENDERING - MIGHT NOT NEED, JUST IMPORT ON THE VECTOR BY CORRECT ORDER
* HANDLE MOVING WINDOW
* TRANSLATE KEY VELOCITY TO ANIMATION
* PEDAL KEYS NOT ON TEXTURES
* PEDAL ANIMATION
* KEY NAME ON PRESSED KEYS
* 7. DO PIANO ANIMATION FOR MOUSE
* 8. REMAKE CHORD RECOGNITION FOR CPP
* IMPLEMENT MENU
* IMPLEMENT SETTINGS
* 9. IMPLEMENT SCREEN RESOLUTIONS + FULLSCREEN
* 88 key setting
* 9. ETC.
*/

#include "Game.h" 

#define SDL_FLAGS SDL_INIT_VIDEO

// This function initializes the SDL engine and all its resources.
bool SDL_Init(SDL* sdl) {

    CHECK_RESULT(sdl->Init(), "Error initializing SDL:");
    sdl->SetRunning(true);

    return true;  /* carry on with the program! */
}

// This function frees the SDL engine and all its resources.
void SDL_Free(SDL* sdl) {
    sdl->~SDL();
    sdl = NULL;
    if (sdl) {
		delete sdl;
    }
}

// This function is called once per frame to render the frame.
void SDL_Run(SDL* sdl) {

    while (sdl->IsRunning()) {
		float startTime = (float)SDL_GetTicks();

        sdl->Simulation();

        sdl->ClearFrame();

        sdl->Rendering();

        sdl->RenderFrame();

        float FTime = SDL_GetTicks() - startTime; 

		if (1000.0f / sdl->GetMaxFPS() > FTime) {
			SDL_Delay((1000.0f / sdl->GetMaxFPS()) - FTime);  // Delay to maintain the desired frame rate
		}
    }

    
}

// This function is the entry point of the game. It initializes the SDL engine, runs the game loop, and cleans up resources.
int game() {

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    SDL* engine = new SDL();

    if (SDL_Init(engine)) {
        SDL_Run(engine);
    }

    SDL_Free(engine);

    return true;
}

#ifdef NDEBUG
// Release mode - no console window
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
    (void)hInstance; (void)hPrevInstance;
    (void)lpCmdLine; (void)nCmdShow;

    SDL_SetMainReady();

    return game();
}

#else
// Debug mode - keep console window
int main(int argc, char* argv[]) {
    (void)argc; (void)argv;  // Silence unused warnings

    SDL_SetMainReady();

    return game();
}
#endif

