#ifndef SDL_H
#define SDL_H

#include "../Keyboard/pianokeyboard.h"
#include "Animations.h"
#include "PerformanceBenchmark.h"
#include "Clock.h"
#include "SDL_Textures.h"
#include "../Scenes/Scene.h"
#include "../Scenes/Menus.h"
#include "Settings/Settings.h"
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

    void SetActiveScene(Scene* scene);
    Scene* GetActiveScene();
    void SetScenes(std::vector<Scene*> scene);
    std::vector<Scene*> GetScenes();
    void DestroyScenes();

    void DestroyScene(Scene* scene);

    bool UpdateScreen();

    void GameEvents();

    bool ScenesSimulation();
    bool Scenes();

	// variables
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool isRunning;

	WindowSettings windowSettings;

	Clock clock;

    Animations* animations;
    
    TTF_Font* mainFont;

	PerfBench* perfBench;

	PianoKeyboard* piano;

    std::vector<Scene*> scenes; // <ActiveScene, Scene1, Scene2, ...>
};

#endif