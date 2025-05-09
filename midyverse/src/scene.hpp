#ifndef SCENE_H
#define SCENE_H

#include "libs.hpp"  

#define TEXTURE_LOAD 0
#define TEXTURE_LOADED 1
#define TEXTURE_FREE 2

#define SCENE_LOAD 0
#define SCENE_LOADED 1
#define SCENE_FREE 2

// struct for all the info need for a texture
struct TextureData  
{  
	SDL_Texture* texture;
	std::string location;  
	int coordinates[8]; // x1, y1, w1, z1, x2, y2, w2, z2  
};  

class Scene  
{  
public:  
   Scene();  
   ~Scene();  
   Scene(std::vector<TextureData> loadTextures);

   int GetSceneProgress();
   void SetSceneProgress(int progress);
   int GetTextureProgress();
   void SetTextureProgress(int progress);

   // this is to know if current scene should have the piano rendered by engine or not
   // doing this to avoid to load and deload piano multiple times
   bool isPiano();
   void SetPiano(bool progress);

   std::vector<TextureData> GetTextures(); 
   void SetTextures(std::vector<TextureData> textures);

   bool FreeTextures();  

private:  
	bool isPianoOn;

	std::vector<TextureData> loadTextures; 
	std::vector<std::string> freeTextures;

	int textureProgress;
	int sceneLoaded;
};

#endif