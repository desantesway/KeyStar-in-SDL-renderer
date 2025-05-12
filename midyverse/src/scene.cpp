#include "scene.hpp"

// This function initializes the scene with given textures
Scene::Scene(std::vector<TextureData> loadTextures) {
    this->isPianoOn = false;
    this->sceneLoaded = false;
    this->loadTextures = loadTextures;
    this->textureLoaded = false;
}
// This function initializes the scene
Scene::Scene() {
    this->isPianoOn = false;
    this->sceneLoaded = false;
    this->textureLoaded = false;
}

// This function destroys the scene and frees memory
Scene::~Scene() { 
    this->sceneLoaded = false;
    FreeTextures(); 
}

bool Scene::IsSceneLoaded() {
    return this->sceneLoaded;
}
void Scene::SetSceneLoaded(int progress) {
    this->sceneLoaded = progress;
}
bool Scene::IsTextureLoaded() {
    return this->textureLoaded;
}
void Scene::SetTextureLoaded(int progress) {
    this->textureLoaded = progress;
}
bool Scene::IsPiano() {
    return this->isPianoOn;
}
void Scene::SetPiano(bool progress) {
    this->isPianoOn = progress;
}

// This function returns the textures of the scene
std::vector<TextureData> Scene::GetTextures()
{
    return this->loadTextures;
}
// This function sets the textures of the scene
void Scene::SetTextures(std::vector<TextureData> textures)
{
    this->loadTextures = textures;
}

// This function frees the textures of the scene 
bool Scene::FreeTextures() {
    for (TextureData& textureData : GetTextures()) {
        if (textureData.texture) {
            SDL_DestroyTexture(textureData.texture);
            textureData.texture = NULL;
        }
    }
    loadTextures.clear();
	this->textureLoaded = false;
    return true;
}