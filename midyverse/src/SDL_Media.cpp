#include "SDL_Engine.hpp"

SDL_Texture* SDL::GetBackground() { return background; }
void SDL::SetBackground(const char* location) { SetTexture(this->background, location); }

void SDL::FreeBackground() { FreeTexture(this->background); }
void SDL::FreeTexture(SDL_Texture*& texture) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

void SDL::SetTexture(SDL_Texture*& texture, const char* location) { texture = IMG_LoadTexture(GetRenderer(), location); }

bool SDL::FreeAllMedia()
{
    CHECK_RESULT(FreeInitMedia(), "Error freeing media: ");

    return true;
}

bool SDL::FreeInitMedia() {
    FreeBackground();

    CHECK_RESULT(GetBackground(), "Error freeing texture: ");

    return true;
}

bool SDL::LoadInitMedia() {

    SetBackground((this->images_location + std::string("background.jpg")).c_str());

    CHECK_RESULT(GetBackground(), "Error loading texture: ");

    return true;
}

bool SDL::RenderTexture(SDL_Texture* texture) {

    SDL_RenderTexture(this->renderer, texture, NULL, NULL);  /* render the texture. */

    return true;
}

bool SDL::RenderTexture(SDL_Texture* texture, float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2) {

    SDL_FRect* rect1 = new SDL_FRect();
    rect1->x = x1;
    rect1->y = y1;
    rect1->w = w1;
    rect1->h = h1;

    SDL_FRect* rect2 = new SDL_FRect();
    rect2->x = x2;
    rect2->y = y2;
    rect2->w = w2;
    rect2->h = h2;

    SDL_RenderTexture(this->renderer, texture, rect1, rect2);  /* render the texture. */

    return true;
}