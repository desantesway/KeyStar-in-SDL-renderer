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
    
    void FreeTexture(SDL_Texture*& texture);

    // This function sets the texture to the given location.
    SDL_Texture* LoadTexture(SDL_Texture*& texture, std::string location);

    // This function initializes SDL and creates a window and renderer.
    bool Init(std::string title, int width, int height, int fullscreen);
    bool Init();

    // This function is called once per frame to render the frame.
    bool RenderFrame();

    // This function is called to clear the frame.
    bool ClearFrame();

    // This function renders the texture to the screen.
    bool RenderTexture(SDL_Texture* texture);

    // This function renders the texture to the screen.
    bool RenderTexture(SDL_Texture* texture, float x1, float y1, float w1, float h1,
        float x2, float y2, float w2, float h2);

    // This function updates the screen if resolution changed by user input.
    bool UpdateScreen();

    void GameEvents();


    // HANDLE ERRORS
    bool LoadTextures(Scene* scene);
    bool StartScene(Scene* scene);

    bool RenderScene(Scene* scene);
    bool RenderTextures(std::vector<TextureData> texture_data);

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

	Scene* scene1;
	//std::map<std::string, SDL_Texture*> Textures; // idk yet
    
};

#endif