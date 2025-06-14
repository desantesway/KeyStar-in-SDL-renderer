#ifndef SDL_H
#define SDL_H

#include "Scene.h"
#include "../Keyboard/pianokeyboard.h"
#include "Animations.h"
#include "PerformanceBenchmark.h"
#include "Clock.h"
#include <unordered_map>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

class SDL
{
public:

    SDL();
    ~SDL();

    bool SetRunning(bool isRunning);
    bool IsRunning();

    bool RenderFrame();
    bool ClearFrame();
    bool Simulation();
    bool Rendering();

    // This functions initializes SDL and creates a window and renderer.
    bool Init(std::string title, int width, int height, int fullscreen);
    bool Init();

    float GetMaxFPS();

private:

    SDL_Renderer* GetRenderer();
    SDL_Window* GetWindow();

    std::string GetName();
    void SetName(std::string string);
    int GetWidth();
    int GetHeight();
    int GetFullscreen();
    void SetFullscreen(int fullscreen);
    void SetWidth(int width);
    void SetHeight(int height);

    void SetActiveScene(Scene* scene);
    Scene* GetActiveScene();
    void SetScene(int scene);
    int GetScene();

    void FreeTexture(SDL_Texture*& texture);
    void DestroyScene(Scene* scene);

    // This function sets the texture to the given location.
    SDL_Texture* LoadTexture(SDL_Texture*& texture, std::string location);

    bool UpdateScreen();

    void GameEvents();

    SDL_FRect* GenerateFRect(float x, float y, float w, float h);

    bool RenderTexture(SDL_Texture* texture);
    bool RenderTexture(SDL_Texture* texture, SDL_FRect* rect1, SDL_FRect* rect2);
    bool RenderTexture(SDL_Texture* texture, float x1, float y1, float w1, float h1,
        bool rect);
    bool RenderTexture(SDL_Texture* texture, float x1, float y1, float w1, float h1,
        float x2, float y2, float w2, float h2);

    bool Scenes();
    bool LoadTextures(Scene* scene);

    bool StartScene(Scene* scene);

    bool RenderScene(Scene* scene);
    bool RenderTextures(std::vector<TextureData> texture_data);

    bool LoadPianoTextures();
    bool RenderPiano();

    bool ChordsText();

    // Use only for the initialization of the program
    bool Scene1();


	// variables
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::string name;
    bool isRunning;
    int width;
    int height;
    int fullscreen;
    bool screen_change;

	Clock clock;

    Animations* animations;
    
    TTF_Font* mainFont;

	PerfBench* perfBench;

	PianoKeyboard* piano;

    int scene;
    Scene* activeScene;
	Scene* scene1;
};

#endif