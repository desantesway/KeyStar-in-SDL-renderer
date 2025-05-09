/* TODOS:
* PRIORITY IN TEXTURE RENDERING - MIGHT NOT NEED, JUST IMPORT ON THE VECTOR BY CORRECT ORDER
* 3. FREE THE MEDIA 
* 2. CREATE CLASS FOR KEYBOARD
* * FPS COUNTER
* 3. CREATE CLASS FOR MIDI -> DETECT KEYS
* 4. CREATE CLASS FOR PIANO -> OCATVE (KEYS) + PEDAL
* 5. RENDER PIANO
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
		sdl->GameEvents();  // Call the game events function

        sdl->ClearFrame();

        sdl->UpdateScreen();

        if (sdl->GetScene() == 1) {
            sdl->Scene1(); // Start the scene
        }
        //sdl->RenderInit();

        sdl->RenderFrame();
    }
    //VOID_CHECK_RESULT(!sdl->FreeAllMedia(), "Error freeing media:");
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

