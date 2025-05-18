#ifndef SDL_H
#define SDL_H

#include "main.hpp"
#include "scene.hpp"
#include "pianokeyboard.hpp"
#include "clock.hpp"
#include <unordered_map>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>



enum class AnimationCurve {
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    EaseOutBounce
};

struct Animation {
	bool direction; // true for forward, false for reverse
    double duration;    // total duration of animation in seconds
    double progress;    // current progress [0, 1]
    double elapsed;     // time elapsed in seconds
    bool finished;      // whether animation is complete
    AnimationCurve curve;
};

class SDL
{
public:

    SDL();
    ~SDL();

    SDL_Renderer* GetRenderer();
    SDL_Window* GetWindow();

    bool SetRunning(bool isRunning);
    bool IsRunning();
    std::string GetName();
    void SetName(std::string string);
    int GetWidth();
    int GetHeight();
    int GetFullscreen();
    void SetFullscreen(int fullscreen);
    void SetWidth(int width);
    void SetHeight(int height);

    float GetFPS();
    void SetFPS(float fps);
    float GetMaxFPS();

    bool Simulation();

    void SetActiveScene(Scene* scene);
    Scene* GetActiveScene();
    void SetScene(int scene);
    int GetScene();
    
    void FreeTexture(SDL_Texture*& texture);
    void DestroyScene(Scene* scene);

    // This function sets the texture to the given location.
    SDL_Texture* LoadTexture(SDL_Texture*& texture, std::string location);

    // This functions initializes SDL and creates a window and renderer.
    bool Init(std::string title, int width, int height, int fullscreen);
    bool Init();

    bool RenderFrame();

    bool ClearFrame();

    bool UpdateScreen();

    void GameEvents();

	void CalculateFPS();
	void FPS();

    bool RenderTexture(SDL_Texture* texture, float x1, float y1, float w1, float h1,
        float x2, float y2, float w2, float h2);

    bool Scenes();
    bool LoadTextures(Scene* scene);

    void StartAnimation(std::string name, double duration);
    void ReverseAnimation(std::string name);
    float AnimationState(std::string name);
    void Animate();

    bool StartScene(Scene* scene);

    bool RenderScene(Scene* scene);
    bool RenderTextures(std::vector<TextureData> texture_data);
    
    bool LoadPianoTextures();
    bool RenderPiano();

    bool ChordsText();

	// Use only for the initialization of the program
    bool Scene1();

private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    std::string name;
    bool isRunning;
    int width;
    int height;
    int fullscreen;
    bool screen_change;

	Clock clock;

    float fps; // set to -1 to dehactivate ingame fps
    float maxfps;
    SDL_Texture* FpsTexture;

    std::map<std::string, Animation> animationStates;
    std::vector<std::string> reversedAnimations;

    TTF_Font* mainFont;

	PianoKeyboard* piano;

    int scene;
    Scene* activeScene;
	Scene* scene1;
   
};

#endif