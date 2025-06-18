#ifndef SDL_Textures_H
#define SDL_Textures_H

#include "../Utils/Libs.h"
#include "../Scenes/Scene.h"

bool RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture);
bool RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_FRect* rect1, SDL_FRect* rect2);
bool RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture, float x1, float y1, float w1, float h1,
    bool rect);
bool RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture, float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2);
bool RenderTextures(SDL_Renderer* renderer, std::vector<TextureData> texture_data);

// This function sets the texture to the given location.
SDL_Texture* LoadTexture(SDL_Renderer* renderer, SDL_Texture*& texture, std::string location);
void FreeTexture(SDL_Texture*& texture);

bool LoadTextures(SDL_Renderer* renderer, Scene* scene);

bool StartScene(SDL_Renderer* renderer, Scene* scene);
bool RenderScene(SDL_Renderer* renderer, Scene* scene);

#endif
