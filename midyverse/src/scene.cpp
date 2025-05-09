#include "scene.hpp"

// This function initializes the scene with given textures
Scene::Scene(std::vector<TextureData> loadTextures) {
    this->isPianoOn = false;
    this->sceneLoaded = SCENE_LOAD;
    SetTextures(loadTextures);
    this->textureProgress = TEXTURE_LOAD;
}
// This function initializes the scene
Scene::Scene() {
    this->isPianoOn = false;
    this->sceneLoaded = SCENE_LOAD;
    this->textureProgress = TEXTURE_LOAD;
}

// This function destroys the scene and frees memory
Scene::~Scene() { FreeTextures(); }


int Scene::GetSceneProgress() {
    return this->sceneLoaded;
}
void Scene::SetSceneProgress(int progress) {
    this->sceneLoaded = progress;
}
int Scene::GetTextureProgress() {
    return this->textureProgress;
}
void Scene::SetTextureProgress(int progress) {
    this->textureProgress = progress;
}
std::vector<TextureData> Scene::GetTextures()
{
    return this->loadTextures;
}
void Scene::SetTextures(std::vector<TextureData> textures)
{
    this->loadTextures = textures;
}
bool Scene::isPiano() {
    return this->isPianoOn;
}
void Scene::SetPiano(bool progress) {
    this->isPianoOn = progress;
}

// This function frees the scene textures
bool Scene::FreeTextures() {

    return true;
}