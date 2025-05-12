/* TODOS:
* PRIORITY IN TEXTURE RENDERING - MIGHT NOT NEED, JUST IMPORT ON THE VECTOR BY CORRECT ORDER
* 3. RENDER FPS INGAME
* 2. CREATE CLASS FOR KEYBOARD
* 3. CREATE CLASS FOR MIDI -> DETECT KEYS
* 4. CREATE CLASS FOR PIANO -> OCATVE (KEYS) + PEDAL
* 5. RENDER PIANO
* HANDLE MOVING WINDOW
* 6. MAKE PIANO ANIMATIONS WITH KEYS
* 7. DO ABOVE FOR MOUSE
* 8. CHORD RECOGNITION
* 9. IMPLEMENT SCREEN RESOLUTIONS + FULLSCREEN
* 9. ETC.
*/

#include "main.hpp" 

#define SDL_FLAGS SDL_INIT_VIDEO

// This function initializes the SDL engine and all its resources.
// SCENE SHOULD BE 1 IS LOADED HERE
bool SDL_Init(SDL* sdl) {

    sdl->SetRunning(true);
    CHECK_RESULT(sdl->Init(), "Error initializing SDL:");

    SDL_SetRenderDrawColor(sdl->GetRenderer(), 54, 54, 54, SDL_ALPHA_OPAQUE_FLOAT);

    return true;  /* carry on with the program! */
}

// This function frees the SDL engine and all its resources.
void SDL_Free(SDL* sdl) {
    if (sdl) {
		delete sdl;
    }
}

// This function is called once per frame to render the frame.
void SDL_Run(SDL* sdl) {
    while (sdl->IsRunning()) {
		float startTime = SDL_GetTicks();  // Get the start time of the frame

		sdl->GameEvents();  // Call the game events function

        sdl->ClearFrame();

        sdl->UpdateScreen();

		sdl->FPS();

        sdl->Scenes();

        sdl->RenderFrame();

        float FTime = SDL_GetTicks() - startTime; 

		if (1000.0f / sdl->GetMaxFPS() > FTime) {
			SDL_Delay((1000.0f / sdl->GetMaxFPS()) - FTime);  // Delay to maintain the desired frame rate
		}
    }

    sdl->~SDL();
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

