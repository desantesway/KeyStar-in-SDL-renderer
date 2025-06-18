#include "MainMenu.h"

// This function is the first scene presented to the user.
Scene* MainMenu(std::vector<Scene*>* scenes, int height) {

    scenes->push_back(new Scene());
    if (scenes->size() == 1) {
        scenes->push_back(scenes->at(0));
    }

    if (!scenes->at(1)->IsSceneLoaded()) {

        std::vector<TextureData> Textures;
        Textures.push_back({ NULL, CAT(ASSETS_IMAGES_PATH,
            CAT(SCENE1_FOLDER,
                CAT(SCENE1_BACKGROUND_FOLDER,
                    CAT(
                        CAT(std::to_string(height), "p_") , SCENE1_BACKGROUND)))), NULL, NULL });


        scenes->at(1)->SetTextures(Textures);


        scenes->at(1)->SetPiano(true);
        scenes->at(1)->SetSceneLoaded(true);

        return scenes->at(1);

    }

    return NULL;
}