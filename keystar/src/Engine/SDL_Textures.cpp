
#include "SDL_Textures.h"

bool RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture) {

    CHECK_RESULT(SDL_RenderTexture(renderer, texture, NULL, NULL), "Error rendering texture f2: ");

    return true;
}

bool RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture, float x1, float y1, float w1, float h1,
    bool rect) {

    SDL_FRect* nrect = new SDL_FRect();
    nrect->x = x1;
    nrect->y = y1;
    nrect->w = w1;
    nrect->h = h1;

    if (rect) {
        CHECK_RESULT(SDL_RenderTexture(renderer, texture, nrect, NULL), "Error rendering texture f2: ");
    }
    else {
        CHECK_RESULT(SDL_RenderTexture(renderer, texture, NULL, nrect), "Error rendering texture f2: ");
    }

    /* render the texture. */

  // Clean up allocated memory
    delete nrect;

    return true;
}

bool RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_FRect* rect1, SDL_FRect* rect2) {

    CHECK_RESULT(SDL_RenderTexture(renderer, texture, rect1, rect2), "Error rendering texture f2: ");  /* render the texture. */

    return true;
}

bool RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture, float x1, float y1, float w1, float h1,
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

    CHECK_RESULT(SDL_RenderTexture(renderer, texture, rect1, rect2), "Error rendering texture f2: ");  /* render the texture. */

    // Clean up allocated memory
    delete rect1;
    delete rect2;

    return true;
}

// This function renders all the textures of the scene to the screen.
bool RenderTextures(SDL_Renderer* renderer, std::vector<TextureData> texture_data) {
    for (TextureData& textureData : texture_data) {
        if (textureData.texture) {
            CHECK_RESULT(RenderTexture(renderer, textureData.texture,
                textureData.rect1, textureData.rect2),
                CAT("Error rendering texture: ", textureData.location));

        }
    }
    return true;
}

// This function loads the texture from the given location.
SDL_Texture* LoadTexture(SDL_Renderer* renderer, SDL_Texture*& texture, std::string location) {
    return texture = IMG_LoadTexture(renderer, StrToPtr(location));
}

// This function frees the texture and sets it to NULL.
void FreeTexture(SDL_Texture*& texture) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

// This function loads all the textures of the scene.
bool LoadTextures(SDL_Renderer* renderer, Scene* scene) {
    std::vector<TextureData> new_textures;
    SDL_Texture* texture = NULL;

    for (TextureData& textureData : scene->GetTextures()) {

        textureData.texture = LoadTexture(renderer, texture, textureData.location);
        CHECK_RESULT(textureData.texture, CAT("Error loading texture: ", textureData.location));

        new_textures.push_back(textureData);

        texture = NULL;
    }

    scene->SetTextures(new_textures);
    return true;
}