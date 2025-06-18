#include "Rects.h"

// Function to generate an SDL_Rect with given dimensions
SDL_FRect* GenerateFRect(float x, float y, float w, float h) {
    SDL_FRect* rect = new SDL_FRect();
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    return rect;
}

SDL_Texture* GenerateGradientTexture(SDL_Renderer* renderer, int width, int height,
    SDL_Color top, SDL_Color bottom, float middlePosition = 0.5f) {
    // Clamp middle position between 0.0 and 1.0
    middlePosition = SDL_clamp(middlePosition, 0.0f, 1.0f);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // Set as render target
    SDL_SetRenderTarget(renderer, texture);

    // Clear to transparent
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Draw gradient
    for (int y = 0; y < height; y++) {
        float normalizedY = (float)y / height;
        float ratio;

        if (middlePosition <= 0.0f) {
            ratio = (normalizedY >= 0.0f) ? 1.0f : 0.0f;
        }
        else if (middlePosition >= 1.0f) {
            ratio = (normalizedY >= 1.0f) ? 1.0f : 0.0f;
        }
        else {
            // Calculate ratio based on middle position
            if (normalizedY <= middlePosition) {
                ratio = 0.5f * (normalizedY / middlePosition);
            }
            else {
                ratio = 0.5f + 0.5f * ((normalizedY - middlePosition) / (1.0f - middlePosition));
            }
        }

        Uint8 r = static_cast<Uint8>(top.r + (bottom.r - top.r) * ratio);
        Uint8 g = static_cast<Uint8>(top.g + (bottom.g - top.g) * ratio);
        Uint8 b = static_cast<Uint8>(top.b + (bottom.b - top.b) * ratio);
        Uint8 a = static_cast<Uint8>(top.a + (bottom.a - top.a) * ratio);

        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderLine(renderer, 0.0f, static_cast<float>(y), static_cast<float>(width - 1), static_cast<float>(y));
    }

    // Reset render target
    SDL_SetRenderTarget(renderer, NULL);

    return texture;
}