#ifndef SDL_H
#define SDL_H

#include "main.hpp"
#include "scene.hpp"

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

    void SetScene(int scene);
    int GetScene();
    
    void FreeTexture(SDL_Texture*& texture);

    // This function sets the texture to the given location.
    SDL_Texture* LoadTexture(SDL_Texture*& texture, std::string location);

    // This functions initializes SDL and creates a window and renderer.
    bool Init(std::string title, int width, int height, int fullscreen);
    bool Init();

    bool RenderFrame();

    bool ClearFrame();

    bool UpdateScreen();

    void GameEvents();

    bool RenderTexture(SDL_Texture* texture);

    bool RenderTexture(SDL_Texture* texture, float x1, float y1, float w1, float h1,
        float x2, float y2, float w2, float h2);

    bool LoadTextures(Scene* scene);
    bool StartScene(Scene* scene);

    bool RenderScene(Scene* scene);
    bool RenderTextures(std::vector<TextureData> texture_data);

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
    
    int scene;
	Scene* scene1;
   
};

#endif