#ifndef SDL_H
#define SDL_H

#include "main.hpp"
#include "scene.hpp"
#include "pianokeyboard.hpp"
#include <unordered_map>

// Add this struct at the top of the file or in a suitable header:
struct KeyAnimState {
    float animValue;      // 0.0 = not pressed, 1.0 = pressed
    float targetValue;    // 0.0 or 1.0
    Uint32 lastUpdate;    // SDL_GetTicks() of last update
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

    float fps; // set to -1 to dehactivate ingame fps
    float maxfps;
    SDL_Texture* FpsTexture;

    TTF_Font* mainFont;

	PianoKeyboard* piano;
    std::unordered_map<int, KeyAnimState> keyAnimStates;

    int scene;
    Scene* activeScene;
	Scene* scene1;
   
};

#endif