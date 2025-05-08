#ifndef SDL_H
#define SDL_H

#include "SDL3.hpp"
#include <string>
#include "Macros.hpp"
#include "main.hpp"

class SDL
{
public:

    SDL();
    ~SDL();

    SDL_Renderer* GetRenderer();
    SDL_Window* GetWindow();

    bool SetRunning(bool isRunning);
    bool IsRunning();
    const char* GetName();
    void SetName(const char* string);
    int GetWidth();
    int GetHeight();
    int GetFullscreen();
    void SetFullscreen(int fullscreen);
    void SetWidth(int width);
    void SetHeight(int height);

    SDL_Texture* GetBackground();
    void SetBackground(const char* location);

    void FreeBackground();
    void FreeTexture(SDL_Texture*& texture);

    // This function sets the texture to the given location.
    void SetTexture(SDL_Texture*& texture, const char* location);
    bool Init(const char* title, int width, int height, int fullscreen);

    // This function initializes SDL and creates a window and renderer.
    bool Init();

    // This function is called once per frame to render the frame.
    bool RenderFrame();

    // This function is called to clear the frame.
    bool ClearFrame();

    // This function is called to free all media.
    bool FreeAllMedia();

    // This function is called to free the only needed textures.
    bool FreeInitMedia();

    // This function loads the media needed to run the program.
    bool LoadInitMedia();

    // This function renders the texture to the screen.
    bool RenderTexture(SDL_Texture* texture);

    // This function renders the texture to the screen.
    bool RenderTexture(SDL_Texture* texture, float x1, float y1, float w1, float h1,
        float x2, float y2, float w2, float h2);

    // This function renders the startup screen.
    bool RenderInit();

    // This function updates the screen if resolution changed by user input.
    bool UpdateScreen();

    void GameEvents();
private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    const char* name;
    bool isRunning;
    int width;
    int height;
    int fullscreen;
    bool screen_change;

    const char* images_location;

    SDL_Texture* background;
};

#endif