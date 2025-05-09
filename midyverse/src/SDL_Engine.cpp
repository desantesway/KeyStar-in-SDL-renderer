#include "SDL_Engine.hpp"


SDL::SDL() {
    window = NULL;
    renderer = NULL;
    name = "MidyVerse";
    width = 1280;
    height = 720;
    fullscreen = 0;
    screen_change = false;
    isRunning = false;

    scene1 = NULL;
}

SDL::~SDL() { SDL_Quit(); }

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

bool SDL::Init(std::string title, int width, int height, int fullscreen)
{
    SetName(title);  /* set the name of the program. */
    SetFullscreen(fullscreen);  /* set the fullscreen flag. */
    SetWidth(width);  /* set the width. */
    SetHeight(height);  /* set the height. */

    return Init();  /* call the other Init() function. */
}

bool SDL::Init()
{
    SDL_SetAppMetadata(StrToPtr(GetName()), "0.01", "com.midyverse.main");
    
    CHECK_RESULT(SDL_Init(SDL_INIT_VIDEO), "Couldn't initialize SDL: ");

    if (!SDL_CreateWindowAndRenderer(StrToPtr(GetName()), GetWidth(), GetHeight(), GetFullscreen(), &this->window, &this->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return false;
    }

    // icon of the program - only is runned once and then freed
    SDL_Surface* icon_surface = IMG_Load(CAT(ASSETS_IMAGES_PATH,"logo.jpg"));
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

bool SDL::RenderTexture(SDL_Texture* texture) {

    SDL_RenderTexture(this->renderer, texture, NULL, NULL);  /* render the texture. */

    return true;
}

bool SDL::RenderTexture(SDL_Texture* texture, float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2) {

    if ((x1 == y1 == w1 == h1 == 0) && (x2 == y2 == w2 == h2 == 0)){
		RenderTexture(texture);
    }

    SDL_FRect* rect1 = new SDL_FRect();
    rect1->x = x1;
    rect1->y = y1;
    rect1->w = w1;
    rect1->h = h1;

    SDL_FRect* rect2 = new SDL_FRect();
    rect2->x = x2;
    rect2->y = y2;
    rect2->w = w2;
    rect2->h = h2;

    SDL_RenderTexture(this->renderer, texture, rect1, rect2);  /* render the texture. */

    return true;
}

SDL_Texture* SDL::LoadTexture(SDL_Texture*& texture, std::string location) {
    return texture = IMG_LoadTexture(GetRenderer(), StrToPtr(location));
}

void SDL::FreeTexture(SDL_Texture*& texture) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

bool SDL::RenderScene(Scene* scene) {
    
	RenderTextures(scene->GetTextures());

    return true;
}

bool SDL::RenderTextures(std::vector<TextureData> texture_data) {
    for (TextureData& textureData : texture_data) {
        if (textureData.texture) {
            RenderTexture(textureData.texture);
        }
    }
    return true;
}

// Expand this function if added surfaces and etc
bool SDL::StartScene(Scene* scene) {
    
    
    if (this->scene1->GetTextureProgress() == TEXTURE_LOAD) {
        LoadTextures(scene);
        this->scene1->SetTextureProgress(TEXTURE_LOADED);
    }
    
    return true;
}

bool SDL::LoadTextures(Scene* scene) {
	std::vector<TextureData> new_textures;

    for (TextureData& textureData : scene->GetTextures()) {
        SDL_Texture* texture = NULL;
        textureData.texture = LoadTexture(texture, textureData.location);
	    CHECK_RESULT(textureData.texture, "Error loading texture: ");

		new_textures.push_back(textureData);

        std::cout << std::endl;
    }  

	scene->SetTextures(new_textures);
    return true;  
}

bool SDL::Scene1() {

    if (this->scene1 == NULL) {
        this->scene1 = new Scene();
    }

    if (this->scene1->GetSceneProgress() == SCENE_LOAD) {

        std::vector<TextureData> Textures;
        Textures.push_back({ NULL, CAT(ASSETS_IMAGES_PATH, "background.jpg"), {0, 0, 0, 0, 0, 0, 0, 0} });

        scene1->SetTextures(Textures);
        
        StartScene(scene1);
    
        this->scene1->SetSceneProgress(SCENE_LOADED);
    }
    if (this->scene1->GetTextureProgress() == SCENE_LOADED) {
        this->RenderScene(scene1);
    }

    return true;
}