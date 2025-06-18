#ifndef SCENE_H
#define SCENE_H

#include "../Utils/Libs.h"  
#include "../Utils/Macros.h" 

// struct for all the info need for a texture
struct TextureData  
{  
	SDL_Texture* texture;
	std::string location;  
	SDL_FRect* rect1; // x1, y1, w1, z1
	SDL_FRect* rect2; // x2, y2, w2, z2  
};  

class Scene  
{  
public:  
   Scene();  
   ~Scene();  
   Scene(std::vector<TextureData> loadTextures);

   bool IsSceneLoaded();
   void SetSceneLoaded(int progress);
   bool IsTextureLoaded();
   void SetTextureLoaded(int progress);
   bool GetDetectKeys();
   void SetDetectKeys(bool progress);

   SDL_Texture* GetChordReconTex();
   void SetChordReconTex(SDL_Texture* tex);

   // this is to know if current scene should have the piano rendered by engine or not
   // doing this to avoid to load and deload piano multiple times
   bool IsPiano();
   void SetPiano(bool progress);

   std::vector<TextureData> GetTextures(); 
   void SetTextures(std::vector<TextureData> textures);

   bool FreeTextures();  

   //bool StartScene(SDL_Renderer* renderer);

private:  
	bool isPianoOn;

	std::vector<TextureData> offlineTextures; 

	bool textureLoaded;
	bool sceneLoaded;

	bool detectKeys;
	SDL_Texture* chordReconTex; // had to be separate cuz of the constant changing of the texture in the scene
};

#endif