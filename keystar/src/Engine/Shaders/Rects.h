#ifndef SHADERS_RECTS_H
#define SHADERS_RECTS_H 

#include "../../Utils/Libs.h"

SDL_FRect* GenerateFRect(float x, float y, float w, float h);
SDL_Texture* GenerateGradientTexture(SDL_Renderer* renderer, int width, int height, SDL_Color top, SDL_Color bottom);

#endif