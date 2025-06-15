#include "Rects.h"

SDL_FRect* GenerateFRect(float x, float y, float w, float h) {
    SDL_FRect* rect = new SDL_FRect();
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    return rect;
}

SDL_Texture* GenerateGradientTexture(SDL_Renderer* renderer, int width, int height, SDL_Color top, SDL_Color bottom) {
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	// Set as render target
	SDL_SetRenderTarget(renderer, texture);

	// Clear to transparent
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// Draw gradient
	for (int y = 0; y < height; y++) {
		float ratio = (float)y / height;
		Uint8 r = top.r + (bottom.r - top.r) * ratio;
		Uint8 g = top.g + (bottom.g - top.g) * ratio;
		Uint8 b = top.b + (bottom.b - top.b) * ratio;
		Uint8 a = top.a + (bottom.a - top.a) * ratio;

		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderLine(renderer, 0, y, width - 1, y);
	}

	// Reset render target
	SDL_SetRenderTarget(renderer, NULL);

	return texture;
}