#include "scene.hpp"

Scene::Scene(std::vector<TextureData> LoadTextures) {
    this->isPianoOn = false;
    this->sceneLoaded = SCENE_LOAD;
    LoadMedia(LoadTextures);
    this->textureProgress = TEXTURE_LOAD;
}
Scene::Scene() {
    this->isPianoOn = false;
    this->sceneLoaded = SCENE_LOAD;
    this->textureProgress = TEXTURE_LOAD;
}
Scene::~Scene() { FreeMedia(); }

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

bool Scene::LoadMedia(std::vector<TextureData> LoadTextures)
{
    this->loadTextures = LoadTextures;
    return true;
}

bool Scene::FreeMedia() {

    return true;
}