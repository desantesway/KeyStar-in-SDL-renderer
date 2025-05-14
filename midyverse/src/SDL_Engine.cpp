#include "SDL_Engine.hpp"

SDL::SDL() {
    this->window = NULL;
    this->renderer = NULL;
    this->name = "MidyVerse";
    this->width = 1920;
    this->height = 1080;
    this->fullscreen = 0;
    this->screen_change = false;
    this->isRunning = false;
    this->fps = -1.0f;
    this->maxfps = 120.0f;
    this->mainFont = NULL;
    this->FpsTexture = NULL;

    this->piano = NULL;

    this->scene = 1;
    this->activeScene = NULL;
    this->scene1 = NULL;
}

SDL::~SDL() {

    if (this->mainFont) {
        TTF_CloseFont(this->mainFont);
        this->mainFont = NULL;
    }
    if (this->FpsTexture) {
        SDL_DestroyTexture(this->FpsTexture);
        this->FpsTexture = NULL;
    }

    if (this->piano) {
		this->piano->~PianoKeyboard();
        delete this->piano;
        this->mainFont = NULL;
    }

    this->SetScene(-1);
    this->Scenes();

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

std::string SDL::GetName() { return name; }
void SDL::SetName(std::string string) { this->name = string; }
int SDL::GetWidth() { return width; }
int SDL::GetHeight() { return height; }
int SDL::GetFullscreen() { return fullscreen; }
void SDL::SetFullscreen(int fullscreen) { this->fullscreen = fullscreen; screen_change = true; }
void SDL::SetWidth(int width) { this->width = width; screen_change = true; }
void SDL::SetHeight(int height) { this->height = height; screen_change = true; }

void SDL::SetScene(int scene) { this->scene = scene; }
int SDL::GetScene() { return this->scene; }
void SDL::SetActiveScene(Scene* scene) { this->activeScene = scene; }
Scene* SDL::GetActiveScene() { return this->activeScene; }

// Sets the variables before starting the SDL engine to then start with the other function
bool SDL::Init(std::string title, int width, int height, int fullscreen)
{
    SetName(title);  /* set the name of the program. */
    SetFullscreen(fullscreen);  /* set the fullscreen flag. */
    SetWidth(width);  /* set the width. */
    SetHeight(height);  /* set the height. */

    return Init();  /* call the other Init() function. */
}

// This function initializes SDL and creates a window and renderer.
bool SDL::Init()
{
    SDL_SetAppMetadata(StrToPtr(GetName()), "0.01", "com.midyverse.main");
    
    CHECK_RESULT(SDL_Init(SDL_INIT_VIDEO), "Couldn't initialize SDL: ");

    CHECK_RESULT(TTF_Init(), "Couldn't initialize SDL TTF: ");

	CHECK_RESULT(SDL_CreateWindowAndRenderer(StrToPtr(GetName()), GetWidth(), GetHeight(), GetFullscreen(), &this->window, &this->renderer), "Couldn't create window/renderer:");

    if (!this->piano) {
        this->piano = new PianoKeyboard();
    }

	CHECK_RESULT(this->piano, "Couldn't create midi: ");
	CHECK_RESULT(this->piano->StartMidi(), "Couldn't start midi: ");

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

    if (screen_change) {
        SDL_SetWindowSize(this->window, GetWidth(), GetHeight());
        SDL_SetWindowFullscreen(this->window, GetFullscreen());
        this->screen_change = false;
    }

    return true;

}

// This function handles the events of the game.
void SDL::GameEvents() {
    static SDL_Event event; 
    if (activeScene) {
        if (activeScene->IsPiano()) {
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
                SetFPS(0);
                break;
            case SDL_SCANCODE_L:
                SetFPS(-1);
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

void SDL::SetFPS(float fps) { this->fps = fps; }
float SDL::GetFPS() { return this->fps; }
float SDL::GetMaxFPS() { return this->maxfps; }

// This function sets, show and calculates the FPS of the program.
void SDL::FPS() {
    if (GetFPS()!= -1) {
        static int frameCount = 0;
        frameCount++;

        CalculateFPS();

        static SDL_FRect fps_rect;

        if (frameCount == 100) {

            if (this->FpsTexture) {
                SDL_DestroyTexture(this->FpsTexture);
                this->FpsTexture = NULL;
            }
            
            SDL_Surface* fps_surface = TTF_RenderText_Blended(this->mainFont, CAT("FPS: ", std::to_string((int)this->GetFPS())), 0, SDL_COLOR_FPS);
            VOID_CHECK_RESULT(fps_surface, "Error rendering FPS surface: ", SDL_GetError());

            if (fps_surface) {
                fps_rect.x = 5.0f;
                fps_rect.w = (float)fps_surface->w;
                fps_rect.h = (float)fps_surface->h;
            }
            this->FpsTexture = SDL_CreateTextureFromSurface(this->renderer, fps_surface);

            SDL_DestroySurface(fps_surface);
            fps_surface = NULL;

            VOID_CHECK_RESULT(this->FpsTexture, "Error rendering FPS texture: ", SDL_GetError());
            //VOID_CHECK_RESULT(SDL_SetTextureScaleMode(this->FpsTexture, SDL_SCALEMODE_NEAREST), "Error setting FPS texture scale mode: ", SDL_GetError());

            frameCount = 0;
        }

        if (this->FpsTexture) {
            SDL_RenderTexture(this->renderer, this->FpsTexture, NULL, &fps_rect); // Render the FPS texture
        }
	}
    else {
        if (this->FpsTexture) {
            SDL_DestroyTexture(this->FpsTexture);
            this->FpsTexture = NULL;
        }
    }
}

// This function calculates the FPS of the program.
void SDL::CalculateFPS() {
    static const int NUM_SAMPLES = 100;
    static float fps_samples[NUM_SAMPLES];
    static int currentFrame = 0;

	static float prevTick = (float)SDL_GetTicks();
    float currentTick;
    currentTick = (float)SDL_GetTicks();

    float frameTime = currentTick - prevTick;

    if (currentFrame >= NUM_SAMPLES) {
        currentFrame = 0;   
    }
    fps_samples[currentFrame] = frameTime;
	
	
	int count = std::min(currentFrame, NUM_SAMPLES);

    float frameAverage = 0;
    for (int i = 0; i < count; i++) {
        frameAverage += fps_samples[i];
    }
    frameAverage /= count;

	if (frameAverage > 0) {
        this->fps = 1000.0f / frameAverage;
    }
    else {
        this->fps = 0.0f;
    }

	prevTick = currentTick;
    currentFrame++;
}

bool SDL::RenderTexture(SDL_Texture* texture, float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2) {

    SDL_FRect* rect1 = NULL;
    SDL_FRect* rect2 = NULL;

    if (!(x1 == 0 && y1 == 0 && w1 == 0 && h1 == 0)) {
        rect1 = new SDL_FRect();
        rect1->x = x1;
        rect1->y = y1;
        rect1->w = w1;
        rect1->h = h1;
    }
    if (!(x2 == 0 && y2 == 0 && w2 == 0 && h2 == 0)) {
        rect2 = new SDL_FRect();
        rect2->x = x2;
        rect2->y = y2;
        rect2->w = w2;
        rect2->h = h2;
    }

    CHECK_RESULT(SDL_RenderTexture(this->renderer, texture, rect1, rect2), "Error rendering texture f2: ");  /* render the texture. */

    // Clean up allocated memory
    delete rect1;
    delete rect2;

    return true;
}

// This function loads the texture from the given location.
SDL_Texture* SDL::LoadTexture(SDL_Texture*& texture, std::string location) {
    return texture = IMG_LoadTexture(GetRenderer(), StrToPtr(location));
}

// This function frees the texture and sets it to NULL.
void SDL::FreeTexture(SDL_Texture*& texture) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

// This function destroys the scene and frees memory.
void SDL::DestroyScene(Scene* scene) {
	if (scene) {
        scene->~Scene();
        scene = NULL;
	}
}

// This function is called to load/render the scenes depending on the variable scene.
bool SDL::Scenes() {

    switch (this->GetScene()) {
    case -1:
        if (this->activeScene) {
            DestroyScene(this->activeScene);
        }
        if (this->scene1) {
            DestroyScene(this->scene1);
        }
    case 1:
        this->Scene1();
		activeScene = this->scene1;
    }
    if (activeScene->IsPiano()) {
        static std::string location = CAT(
            CAT(ASSETS_IMAGES_PATH, PIANO_FOLDER_PATH),
            CAT(std::to_string(this->piano->GetKeyNum()), PIANO_PATH));
        if (!this->piano->GetPianoTexture()) {
            SDL_Texture* pianoTexture = NULL;
            pianoTexture = LoadTexture(pianoTexture,location);
            CHECK_RESULT(pianoTexture, "Error loading piano texture: ");

            this->piano->SetPianoTexture(pianoTexture, location);

            pianoTexture = NULL;
           
        }
        else {

            CHECK_RESULT(RenderTexture(this->piano->GetPianoTexture(),
                0, 0, this->piano->GetPianoTextureWidth(), this->piano->GetPianoTextureHeight(),
                0, GetHeight() - (this->piano->GetPianoTextureHeight() * GetWidth())/ this->piano->GetPianoTextureWidth(), GetWidth(), (this->piano->GetPianoTextureHeight() * GetWidth()) / this->piano->GetPianoTextureWidth()),
                CAT("Error rendering texture: ", location));
        }
            
    }
    return true;
}

// This function renders the scene to the screen.
bool SDL::RenderScene(Scene* scene) {
    
	RenderTextures(scene->GetTextures());

    return true;
}

// This function renders all the textures of the scene to the screen.
bool SDL::RenderTextures(std::vector<TextureData> texture_data) {
    for (TextureData& textureData : texture_data) {
        if (textureData.texture) {
            CHECK_RESULT(RenderTexture(textureData.texture, 
                textureData.coordinates[0], textureData.coordinates[1], textureData.coordinates[2], textureData.coordinates[3],
                textureData.coordinates[4], textureData.coordinates[5], textureData.coordinates[6], textureData.coordinates[7]),
                CAT("Error rendering texture: ", textureData.location));
        }
    }
    return true;
}

// This function starts the scene and loads all the media needed.
// Expand this function if added surfaces and etc
bool SDL::StartScene(Scene* scene) {
    
    
    if (!scene->IsTextureLoaded()) {
        LoadTextures(scene);
        scene->SetTextureLoaded(true);
    }
    
    return true;
}

// This function loads all the textures of the scene.
bool SDL::LoadTextures(Scene* scene) {
	std::vector<TextureData> new_textures;
    SDL_Texture* texture = NULL;

    for (TextureData& textureData : scene->GetTextures()) {
        
        textureData.texture = LoadTexture(texture, textureData.location);
        CHECK_RESULT(textureData.texture, CAT("Error loading texture: ", textureData.location));

		new_textures.push_back(textureData);

        texture = NULL;
    }  

	scene->SetTextures(new_textures);
    return true;  
}

// This function is the first scene presented to the user.
bool SDL::Scene1() {

    if (this->scene1 == NULL) {
        this->scene1 = new Scene();
    }

    if (!this->scene1->IsSceneLoaded()) {

        std::vector<TextureData> Textures;
        Textures.push_back({ NULL, CAT(ASSETS_IMAGES_PATH, SCENE1_BACKGROUND), {0, 0, 0, 0, 0, 0, 0, 0} });

        scene1->SetTextures(Textures);
        
        StartScene(scene1);

		this->scene1->SetPiano(true);
        this->scene1->SetSceneLoaded(true);
    }
    else {
        this->RenderScene(scene1);
    }

    return true;
}