#include "Shaders.h"

SDL_Texture* MaskTexture(SDL_Renderer* renderer, SDL_Texture*& mask, SDL_Texture*& diffuse)
{
	SDL_Texture* result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 300, 300);
	SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderer, result);
	SDL_SetTextureBlendMode(mask, SDL_BLENDMODE_MOD);
	SDL_SetTextureBlendMode(diffuse, SDL_BLENDMODE_NONE);
	SDL_RenderTexture(renderer, diffuse, NULL, NULL);
	SDL_RenderTexture(renderer, mask, NULL, NULL);
	SDL_SetRenderTarget(renderer, NULL);
	return result;
}