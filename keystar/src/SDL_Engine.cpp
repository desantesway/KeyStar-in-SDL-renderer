#include "SDL_Engine.hpp"

SDL::SDL() {
    this->window = NULL;
    this->renderer = NULL;
    this->name = "KeyStar";
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

    this->SetScene(-1);
    this->Scenes();

    if (this->piano) {
        this->piano->~PianoKeyboard();
        this->piano = NULL;
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
    CHECK_RESULT(LoadPianoTextures(), "Error loading piano textures: ");

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

    Animate();

    Scenes();

    RenderPiano();

    FPS();

    GameEvents();

    return true;
}

bool SDL::Rendering() {
    Scenes();

    RenderPiano();

    FPS();

    GameEvents();

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

SDL_FRect* SDL::GenerateFRect(float x, float y, float w, float h) {
    SDL_FRect* rect = new SDL_FRect();
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
	return rect;
}

bool SDL::RenderTexture(SDL_Texture* texture) {

    CHECK_RESULT(SDL_RenderTexture(this->renderer, texture, NULL, NULL), "Error rendering texture f2: ");

    return true;
}

bool SDL::RenderTexture(SDL_Texture* texture, float x1, float y1, float w1, float h1,
    bool rect) {

    SDL_FRect* nrect = new SDL_FRect();
    nrect->x = x1;
    nrect->y = y1;
    nrect->w = w1;
    nrect->h = h1;

    if (rect) {
        CHECK_RESULT(SDL_RenderTexture(this->renderer, texture, nrect, NULL), "Error rendering texture f2: ");
    }
    else {
        CHECK_RESULT(SDL_RenderTexture(this->renderer, texture, NULL, nrect), "Error rendering texture f2: ");
    }

      /* render the texture. */

    // Clean up allocated memory
    delete nrect;

    return true;
}

bool SDL::RenderTexture(SDL_Texture* texture, SDL_FRect* rect1, SDL_FRect* rect2) {

    CHECK_RESULT(SDL_RenderTexture(this->renderer, texture, rect1, rect2), "Error rendering texture f2: ");  /* render the texture. */

    return true;
}

bool SDL::RenderTexture(SDL_Texture* texture, float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2) {

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
		this->activeScene->SetDetectKeys(true);
		ChordsText();
    }
    if (this->activeScene) {
        
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
                textureData.rect1, textureData.rect2),
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
        Textures.push_back({ NULL, CAT(ASSETS_IMAGES_PATH, SCENE1_BACKGROUND), NULL, NULL});

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

bool SDL::LoadPianoTextures() {
    SDL_Texture* pianoTexture = NULL;
	std::string pianoFolderPath = CAT(ASSETS_IMAGES_PATH, 
                                    CAT(PIANO_FOLDER_PATH, 
                                        CAT(std::to_string(GetHeight()), "p_")
                                    )
                                  );
    std::string location = CAT(pianoFolderPath, RWHITE_PATH);
    pianoTexture = LoadTexture(pianoTexture, location);
    CHECK_RESULT(pianoTexture, "Error loading piano RWhiteKey texture: ");
    this->piano->SetRWhiteKey(this->piano->LoadKeyTex(this->piano->GetRWhiteKey(), pianoTexture, location));

    location = CAT(pianoFolderPath, LWHITE_PATH);
    pianoTexture = LoadTexture(pianoTexture, location);
    CHECK_RESULT(pianoTexture, "Error loading piano LWhiteKey texture: ");
    this->piano->SetLWhiteKey(this->piano->LoadKeyTex(this->piano->GetLWhiteKey(), pianoTexture, location));

    location = CAT(pianoFolderPath, WHITE_MID_PATH);
    pianoTexture = LoadTexture(pianoTexture, location);
    CHECK_RESULT(pianoTexture, "Error loading piano MidWhiteKey texture: ");
    this->piano->SetMidWhiteKey(this->piano->LoadKeyTex(this->piano->GetMidWhiteKey(), pianoTexture, location));

    location = CAT(pianoFolderPath, WHITE_ROUNDMID_PATH);
    pianoTexture = LoadTexture(pianoTexture, location);
    CHECK_RESULT(pianoTexture, "Error loading piano RoundWhiteKey texture: ");
    this->piano->SetRoundWhiteKey(this->piano->LoadKeyTex(this->piano->GetRoundWhiteKey(), pianoTexture, location));

    location = CAT(pianoFolderPath, BLACK_KEY_PATH);
    pianoTexture = LoadTexture(pianoTexture, location);
    CHECK_RESULT(pianoTexture, "Error loading piano BlackKey texture: ");
    this->piano->SetBlackKey(this->piano->LoadKeyTex(this->piano->GetBlackKey(), pianoTexture, location));

    location = CAT(pianoFolderPath, BLACK_SHADOW_PATH);
    pianoTexture = LoadTexture(pianoTexture, location);
    CHECK_RESULT(pianoTexture, "Error loading piano BlackKeyShadow texture: ");
    this->piano->SetBlackKeyShadow(this->piano->LoadKeyTex(this->piano->GetBlackKeyShadow(), pianoTexture, location));

    location = CAT(pianoFolderPath, BLACK_BLEND_PATH);
    pianoTexture = LoadTexture(pianoTexture, location);
    CHECK_RESULT(pianoTexture, "Error loading piano BlackBlendKey texture: ");
    this->piano->SetBlackBlendKey(this->piano->LoadKeyTex(this->piano->GetBlackBlendKey(), pianoTexture, location));

    pianoTexture = NULL;

    return true;
}

void SDL::StartAnimation(std::string name, double duration, AnimationCurve curve) {
    if (!animationStates.count(name)) {
        animationStates.insert({ name, { true, duration, 0.0, 0.0, false, curve} });
    }
    
}

float SDL::AnimationState(std::string name) {
    if (animationStates.count(name)) {
		return animationStates[name].progress;
    }
    else {
        return 0;
    }
    return 0;
}

void SDL::ReverseAnimation(std::string name, AnimationCurve curve) {
    if (animationStates.count(name) && !(count(reversedAnimations.begin(), reversedAnimations.end(), name) > 0)) {
        Animation& original = animationStates[name];
        Animation reversed;

        // Preserve original duration
        reversed.duration = original.duration;  

        // Start from current progress (reversed)
        reversed.progress = original.progress;
        reversed.elapsed = reversed.progress * reversed.duration;
		reversed.curve = curve; // Preserve the curve type flip this if ease in to out etc

        // Flip direction and reset finished state
        reversed.direction = !original.direction;
        reversed.finished = false;

        // Update the animation
        animationStates.erase(name);
		animationStates.insert({ name, reversed });
		reversedAnimations.push_back(name);
    }
}

// 2. Helper function to apply the animation curve
static double ApplyCurve(double t, AnimationCurve curve) {
    switch (curve) {
    case AnimationCurve::EaseIn:
        return t * t;
    case AnimationCurve::EaseOut:
        return t * (2 - t);
    case AnimationCurve::EaseInOut:
        return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
    case AnimationCurve::EaseOutBounce:
    {
        // Apply ease-out first
        double easedT = t * (2 - t);

        // Bounce parameters (adjust these for different effects)
        const double bounceStart = 0.5;  // When bounce kicks in (0-1)
        const int bounces = 6;           // Number of bounces
        const double bouncePower = 0.6;  // How high the bounces go
        const double decay = 0.3;       // How fast bounces shrink (0-1, lower = slower decay)

        if (easedT > bounceStart) {
            double bounceT = (easedT - bounceStart) / (1 - bounceStart);
            // Exaggerated bounce using a decaying sine wave
            double bounce = sin(bounceT * M_PI * bounces) *
                pow(1 - bounceT, decay) *
                bouncePower;
            easedT += bounce;
        }
        return std::max(0.0, std::min(1.0, easedT));
    }
    case AnimationCurve::Linear:
    default:
        return t;
    }
}

// 3. Modify Animate() to use the curve
void SDL::Animate() {
    std::map<std::string, Animation> updatedAnimations;

    for (auto& i : animationStates) {
        Animation& anim = i.second;

        anim.elapsed += anim.direction ? clock.delta : -clock.delta;
        anim.progress = anim.elapsed / anim.duration;

        if (anim.progress >= 1.0 && anim.direction) {
            anim.progress = 1.0;
            anim.finished = true;
        }
        else if (anim.progress <= 0.0 && !anim.direction) {
            anim.progress = 0.0;
            anim.finished = true;
            auto it = std::find(reversedAnimations.begin(), reversedAnimations.end(), i.first);
            if (it != reversedAnimations.end()) {
                reversedAnimations.erase(it);
            }
			continue;
        }

        double clampedProgress = std::max(0.0, std::min(1.0, anim.progress));
        double curvedProgress = ApplyCurve(clampedProgress, anim.curve);
        anim.progress = curvedProgress;

        updatedAnimations.insert({ i.first, anim });
    }

    this->animationStates = updatedAnimations;
}

bool SDL::RenderPiano() {
    if (!activeScene->IsPiano()) return true;

    // Pseudocode plan:
    // 1. For 88 keys, ensure the piano starts at A0 (MIDI 21) and ends at C8 (MIDI 108).
    // 2. For other key counts, center the keys around middle C (MIDI 60) as before.
    // 3. Adjust start_key calculation to snap to A0 for 88 keys, otherwise use the old logic.

    int total_keys = this->piano->GetKeyNum();
    int full_octaves = total_keys / 12;
    int middle_c = 60; // MIDI note number for middle C (C4)

    int start_key = middle_c - total_keys / 2 + 12 * this->piano->GetOctave();

    int remaining_keys = total_keys % 12;
    int white_keys = full_octaves * 7 + std::min(remaining_keys, 7);
    int black_keys = full_octaves * 5 + std::max(remaining_keys - 7, 0);
    double width = static_cast<double>(GetWidth());
    double height = static_cast<double>(GetHeight());
    double shadow_key_width = width / white_keys;
    double white_key_width = width / white_keys;
    double white_key_height = this->piano->GetRoundWhiteKey().h;
    double white_shadow_height = (height * (double)WHITE_SHADOW_HEIGHT) / ASSESTS_RES;
    double shadow_black_key_width = this->piano->GetBlackKeyShadow().w * 49 / piano->GetKeyNum();
    double shadow_black_key_height = this->piano->GetBlackKeyShadow().h;
    double wblack_height = ((height * (double)WHITEB_KEY_HEIGHT) / ASSESTS_RES) + white_shadow_height;
    double black_key_width = this->piano->GetBlackKey().w * 49 / piano->GetKeyNum();
    double black_key_height = this->piano->GetBlackKey().h;
    double black_height = (height * (double)BLACK_SHADOW_HEIGHT) / ASSESTS_RES;

    SDL_SetRenderDrawColor(this->renderer, 229, 229, 229, 255);
    SDL_RenderFillRect(this->renderer, GenerateFRect(0, height - white_shadow_height * 2, width, white_shadow_height * 2));

    // Render all keys in two passes: one for white, one for black (shadows and keys together)
    // Pass 1: White keys and their shadows
    for (int i = 0; i < white_keys; ++i) {

        // White key
        int octave = i / 7;
        int note_in_octave = i % 7;
        int key_pos = start_key + octave * 12 +
            (note_in_octave == 0 ? 0 :
                note_in_octave == 1 ? 2 :
                note_in_octave == 2 ? 4 :
                note_in_octave == 3 ? 5 :
                note_in_octave == 4 ? 7 :
                note_in_octave == 5 ? 9 : 11);
        
        bool pressed = this->piano->GetNotesPlayed()[key_pos].pressed; // make option for if they in pedal aka this->piano->GetNotesPlayed().count(key_pos) > 0 (on black keys too)
        float wx = static_cast<float>(i * white_key_width);
        float ww = (i == white_keys - 1) ? static_cast<float>(width - wx) : static_cast<float>(shadow_key_width);
        SDL_Texture* white_key_texture =
            (i == white_keys - 1) ? this->piano->GetRoundWhiteKey().tex :
            (i % 7 == 0) ? this->piano->GetLWhiteKey().tex :
            (i % 7 == 6) ? this->piano->GetRWhiteKey().tex :
            this->piano->GetMidWhiteKey().tex;

		std::string key_name = CAT("Key", std::to_string(key_pos));
        if (pressed) {
			StartAnimation(key_name, 0.025, AnimationCurve::EaseOut);
            SDL_SetTextureColorMod(white_key_texture, GREEN);
        }
        else {
            ReverseAnimation(CAT("Key", std::to_string(key_pos)), AnimationCurve::EaseOutBounce);
            SDL_SetTextureColorMod(white_key_texture, 255, 255, 255);
        }
        RenderTexture(white_key_texture, wx, height - white_key_height - white_shadow_height, ww, white_key_height +  white_shadow_height*AnimationState(key_name), false);
    }
    SDL_Texture* white_key_texture = NULL;

    SDL_SetTextureColorMod(this->piano->GetBlackBlendKey().tex, GREEN);
    double mult = white_key_width - shadow_black_key_width * 0.5;
    // Pass 2: Black key shadows and black keys
    for (int i = 0; i < black_keys; ++i) {
        // Black key shadow
        float sx = static_cast<float>(mult);
        float sw = static_cast<float>(shadow_black_key_width);
        RenderTexture(this->piano->GetBlackKeyShadow().tex, sx, height - shadow_black_key_height - wblack_height, sw, shadow_black_key_height, false);

        // Black key
        int octave = i / 5;
        int note_in_octave = i % 5;
        int key_pos = start_key + octave * 12 +
            (note_in_octave == 0 ? 1 :
                note_in_octave == 1 ? 3 :
                note_in_octave == 2 ? 6 :
                note_in_octave == 3 ? 8 : 10);
        bool pressed = this->piano->GetNotesPlayed()[key_pos].pressed;

        float bx = static_cast<float>(mult);
        float bw = static_cast<float>(black_key_width);

        SDL_Texture* black_key_texture = NULL;

        std::string key_name = CAT("Key", std::to_string(key_pos));
        if (pressed) { // do a white version of the black 
            StartAnimation(key_name, 0.025, AnimationCurve::EaseOut);
            black_key_texture = this->piano->GetBlackBlendKey().tex;
        } else {
            ReverseAnimation(CAT("Key", std::to_string(key_pos)), AnimationCurve::EaseOutBounce);
            black_key_texture = this->piano->GetBlackKey().tex;
		}
        
        RenderTexture(black_key_texture, bx, height - black_key_height - black_height - wblack_height , bw, black_key_height + black_height *1.03 * AnimationState(key_name), false);

        int keynum = ((i + 1) % 5 == 2 || (i + 1) % 5 == 0) ? 2 : 1;
        mult += white_key_width * keynum;
    }
    SDL_Texture* black_key_texture = NULL;
    return true;
}

bool SDL::ChordsText() {
    if (this->activeScene->GetDetectKeys()) {
		static SDL_FRect chord_rect;
        if (this->piano->AreNotesUpdated()) {

            if (this->activeScene->GetChordReconTex()) {
                SDL_DestroyTexture(this->activeScene->GetChordReconTex());
                this->activeScene->SetChordReconTex(NULL);
            }

            SDL_Surface* chord_surface = TTF_RenderText_Blended(this->mainFont, StrToPtr(this->piano->GetChordPlayed()), 0, SDL_COLOR_FPS); // CHANGE TO OTHER FONT SIZE
            CHECK_RESULT(chord_surface, "Error rendering Chord Recognition surface: ", SDL_GetError());

            if (chord_surface) {
                chord_rect.y = 50.0f;
                chord_rect.x = 50.0f;
                chord_rect.w = (float)chord_surface->w;
                chord_rect.h = (float)chord_surface->h;
            }
            this->activeScene->SetChordReconTex(SDL_CreateTextureFromSurface(this->renderer, chord_surface));

            SDL_DestroySurface(chord_surface);
            chord_surface = NULL;

            CHECK_RESULT(this->activeScene->GetChordReconTex(), "Error rendering FPS texture: ", SDL_GetError());
            //VOID_CHECK_RESULT(SDL_SetTextureScaleMode(this->FpsTexture, SDL_SCALEMODE_NEAREST), "Error setting FPS texture scale mode: ", SDL_GetError());
        }

        if (chord_rect.w != 0 && chord_rect.h != 0) {
            SDL_RenderTexture(this->renderer, this->activeScene->GetChordReconTex(), NULL, &chord_rect);
        }
        

    }
    return true;
}