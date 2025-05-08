#include "SDL_Engine.hpp"

SDL::SDL() {
    window = NULL;
    renderer = NULL;
    name = "MidyVerse";
    width = 1280;
    height = 720;
    fullscreen = 0;
    background = NULL;
    screen_change = false;
    isRunning = false;
    images_location = "assets/images/";
}

SDL::~SDL() { SDL_Quit(); }

SDL_Renderer* SDL::GetRenderer() { return renderer; }
SDL_Window* SDL::GetWindow() { return window; }

bool SDL::SetRunning(bool isRunning) { this->isRunning = isRunning; return isRunning; }
bool SDL::IsRunning() { return isRunning; }

const char* SDL::GetName() { return name; }
void SDL::SetName(const char* string) { this->name = string; }
int SDL::GetWidth() { return width; }
int SDL::GetHeight() { return height; }
int SDL::GetFullscreen() { return fullscreen; }
void SDL::SetFullscreen(int fullscreen) { this->fullscreen = fullscreen; screen_change = true; }
void SDL::SetWidth(int width) { this->width = width; screen_change = true; }
void SDL::SetHeight(int height) { this->height = height; screen_change = true; }

bool SDL::Init(const char* title, int width, int height, int fullscreen)
{
    SetName(title);  /* set the name of the program. */
    SetFullscreen(fullscreen);  /* set the fullscreen flag. */
    SetWidth(width);  /* set the width. */
    SetHeight(height);  /* set the height. */

    return Init();  /* call the other Init() function. */
}

bool SDL::Init()
{
    SDL_SetAppMetadata(GetName(), "0.01", "com.midyverse.main");

    CHECK_RESULT(SDL_Init(SDL_INIT_VIDEO), "Couldn't initialize SDL: ");

    if (!SDL_CreateWindowAndRenderer(GetName(), GetWidth(), GetHeight(), GetFullscreen(), &this->window, &this->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return false;
    }

    // icon of the program - only is runned once and then freed
    SDL_Surface* icon_surface = IMG_Load((this->images_location + std::string("logo.jpg")).c_str());
    CHECK_RESULT(icon_surface, "Couldn't load icon surface: ");
    CHECK_RESULT(SDL_SetWindowIcon(GetWindow(), icon_surface), "Couldn't set window icon: ", SDL_DestroySurface(icon_surface));

    SDL_DestroySurface(icon_surface);

    return true;
}

bool SDL::RenderFrame()
{
    SDL_RenderPresent(GetRenderer());

    return true;
}

bool SDL::ClearFrame()
{
    SDL_RenderClear(GetRenderer());

    return true;
}

bool SDL::RenderInit() {

    RenderTexture(GetBackground());

    return true;
}

bool SDL::UpdateScreen() {

    if (screen_change) {
        SDL_SetWindowSize(this->window, GetWidth(), GetHeight());
        SDL_SetWindowFullscreen(this->window, GetFullscreen());
        screen_change = false;
    }

    return true;

}

void SDL::GameEvents() {
    SDL_Event event; // Declare a local SDL_Event variable  
    while (SDL_PollEvent(&event)) { // Pass the address of the local event variable  
        switch (event.type) { // Use the local event variable  
        case SDL_EVENT_QUIT:
            SetRunning(false);
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (event.key.scancode) {
            case SDL_SCANCODE_ESCAPE:
                SetRunning(false);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}