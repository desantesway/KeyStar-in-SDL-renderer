#include "SDL_Engine.h"

SDL::SDL() {
    this->window = NULL;
    this->renderer = NULL;
    this->isRunning = false;

    this->piano = NULL;

    this->mainFont = NULL;

	this->animations = NULL;
    this->perfBench = NULL;
}

SDL::~SDL() {

    if (this->mainFont) {
        TTF_CloseFont(this->mainFont);
        this->mainFont = NULL;
    }

    this->DestroyScenes();

    if (this->animations) {
        this->animations->~Animations();
        this->animations = NULL;
    }

    if (this->piano) {
        this->piano->~PianoKeyboard();
        this->piano = NULL;
    }

    if (this->perfBench) {
        this->perfBench->~PerfBench();
        this->perfBench = NULL;
    }

    if (this->renderer) {
        SDL_DestroyRenderer(this->renderer);
        this->renderer = NULL;
    }
    if (this->window) {
        SDL_DestroyWindow(this->window);
        this->window = NULL;
    }

    TTF_Quit();
    SDL_Quit();
}

SDL_Renderer* SDL::GetRenderer() { return renderer; }
SDL_Window* SDL::GetWindow() { return window; }

bool SDL::SetRunning(bool isRunning) { this->isRunning = isRunning; return isRunning; }
bool SDL::IsRunning() { return isRunning; }

void SDL::SetScenes(std::vector<Scene*> scene) { this->scenes = scene; }
std::vector<Scene*> SDL::GetScenes() { return this->scenes; }
void SDL::SetActiveScene(Scene* scene) { this->scenes.at(0) = scene; }
Scene* SDL::GetActiveScene() { return this->scenes.at(0); }

void SDL::DestroyScenes() {
    for (Scene* scene : GetScenes()) {
        if (scene != NULL) {
            scene->~Scene();
            scene = NULL;
        }

    }
}

// Sets the variables before starting the SDL engine to then start with the other function
bool SDL::Init(std::string title, int width, int height, int fullscreen)
{
    windowSettings.SetName(title);  /* set the name of the program. */
    windowSettings.SetFullscreen(fullscreen);  /* set the fullscreen flag. */
    windowSettings.SetWidth(width);  /* set the width. */
    windowSettings.SetHeight(height);  /* set the height. */

    return Init();  /* call the other Init() function. */
}

// This function initializes SDL and creates a window and renderer.
bool SDL::Init()
{

    SDL_SetAppMetadata(StrToPtr(windowSettings.GetName()), "0.01", "com.midyverse.main");
    
    CHECK_RESULT(SDL_Init(SDL_INIT_VIDEO), "Couldn't initialize SDL: ");

    CHECK_RESULT(TTF_Init(), "Couldn't initialize SDL TTF: ");

	CHECK_RESULT(SDL_CreateWindowAndRenderer(StrToPtr(windowSettings.GetName()), windowSettings.GetWidth(), windowSettings.GetHeight(), windowSettings.GetFullscreen(), &this->window, &this->renderer), "Couldn't create window/renderer:");

    if (!this->piano) {
        this->piano = new PianoKeyboard();
    }
    if (!this->animations) {
        this->animations = new Animations();
    }
    if (!this->perfBench) {
        this->perfBench = new PerfBench();
    }

	CHECK_RESULT(this->piano, "Couldn't create midi: ");
	CHECK_RESULT(this->piano->StartMidi(), "Couldn't start midi: ");
    CHECK_RESULT(this->piano->LoadPianoTextures(this->GetRenderer(),this->windowSettings.GetHeight()), "Error loading piano textures: ");

    // icon of the program - only is runned once and then freed
    SDL_Surface* icon_surface = IMG_Load(CAT(ASSETS_IMAGES_PATH,ICON_PATH));
    CHECK_RESULT(icon_surface, "Couldn't load icon surface: ");
    CHECK_RESULT(SDL_SetWindowIcon(GetWindow(), icon_surface), "Couldn't set window icon: ", SDL_DestroySurface(icon_surface));

    SDL_DestroySurface(icon_surface);

    // main font
	this->mainFont = TTF_OpenFont(CAT(FONT_PATH, MAIN_FONT_PATH), MAIN_FONT_SIZE);
	CHECK_RESULT(this->mainFont, "Couldn't load font: ", SDL_GetError());

    return true;
}

bool SDL::Simulation() {
    clock.tick();

    UpdateScreen();

    animations->Animate(clock.delta);

    //ScenesSimulation();

    perfBench->BenchmarkSimulation();

    GameEvents();

    return true;
}

bool SDL::Rendering() {
    Scenes();

    this->piano->RenderPiano(this->GetRenderer(), this->animations, this->scenes.at(0)->IsPiano(), this->windowSettings.GetWidth(), this->windowSettings.GetHeight());

    perfBench->BenchmarkRendering(this->GetRenderer(), this->mainFont);

    return true;
}

// This function is called once per frame to render the frame.
bool SDL::RenderFrame()
{ 
    SDL_RenderPresent(GetRenderer());

    return true;
}

// This function is called to clear the frame.
bool SDL::ClearFrame()
{
    SDL_RenderClear(GetRenderer());

    return true;
}

// This function updates the screen if resolution changed by user input.
bool SDL::UpdateScreen() {

    if (windowSettings.DidScreenChange()) {
        SDL_SetWindowSize(this->window, windowSettings.GetWidth(), windowSettings.GetHeight());
        SDL_SetWindowFullscreen(this->window, windowSettings.GetFullscreen());
        windowSettings.SetScreenChange(false);
    }

    return true;

}

// This function handles the events of the game.
void SDL::GameEvents() {
    static SDL_Event event; 
    if (this->scenes.size() > 1) {
        if (this->scenes.at(0)->IsPiano()) {
            this->piano->DetectKeys();
        }
    }
    while (SDL_PollEvent(&event)) {
        switch (event.type) { 
        case SDL_EVENT_QUIT:
            SetRunning(false);
            break;
        case SDL_EVENT_KEY_DOWN: // keyboard
            switch (event.key.scancode) {
            case SDL_SCANCODE_ESCAPE:
                SetRunning(false);
                break;
            case SDL_SCANCODE_SPACE:
                perfBench->SetFPS(0);
                break;
            case SDL_SCANCODE_L:
                perfBench->SetFPS(-1);
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

float SDL::GetMaxFPS() {
	return perfBench->GetMaxFPS();
};

// This function destroys the scene and frees memory.
void SDL::DestroyScene(Scene* scene) {
	if (scene) {
        scene->~Scene();
        scene = NULL;
	}
}

// This function is called to load/render the scenes depending on the variable scene.
bool SDL::ScenesSimulation() {

    return true;
}

// This function is called to load/render the scenes depending on the variable scene.
bool SDL::Scenes() {

    if (this->scenes.size() > 1) {
        this->RenderScene(scenes.at(0));
    }
    else {
        this->Scene1();
        SetActiveScene(this->scenes.at(1));
        this->scenes.at(0)->SetDetectKeys(true);
        this->piano->ChordsText(this->GetRenderer(), this->GetActiveScene(), this->mainFont);
    }

    return true;
}

// This function renders the scene to the screen.
bool SDL::RenderScene(Scene* scene) {
    
	RenderTextures(this->GetRenderer(), scene->GetTextures());

    return true;
}

// This function starts the scene and loads all the media needed.
// Expand this function if added surfaces and etc
bool SDL::StartScene(Scene* scene) {

    if (!scene->IsTextureLoaded()) {
        LoadTextures(this->GetRenderer(), scene);
        scene->SetTextureLoaded(true);
    }
    
    return true;
}

// This function is the first scene presented to the user.
bool SDL::Scene1() {

    this->scenes.push_back(new Scene());
    if (this->scenes.size() == 1) {
        this->scenes.push_back(this->scenes.at(0));
    }

    if (!this->scenes.at(1)->IsSceneLoaded()) {

        std::vector<TextureData> Textures;
        Textures.push_back({ NULL, CAT(ASSETS_IMAGES_PATH, 
            CAT(SCENE1_FOLDER,
                CAT(
                    CAT(std::to_string(windowSettings.GetHeight()), "p_") , SCENE1_BACKGROUND))), NULL, NULL});

        scenes.at(1)->SetTextures(Textures);
        
        StartScene(scenes.at(1));

		this->scenes.at(1)->SetPiano(true);
        this->scenes.at(1)->SetSceneLoaded(true);
        
    }

    return true;
}