#include "libs.hpp"  

#define TEXTURE_LOAD 0
#define TEXTURE_LOADED 1
#define TEXTURE_FREE 2

#define SCENE_LOAD 0
#define SCENE_LOADED 1
#define SCENE_FREE 2

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

   bool isPiano();
   void SetPiano(bool progress);


   std::vector<TextureData> GetTextures(); // Returns the list of textures to load.
   void SetTextures(std::vector<TextureData> textures); // Sets the list of textures to load.

   // This function loads the media needed to run this scene.  
   bool LoadMedia(std::vector<TextureData> loadTextures);

   // This function frees the scene textures.  
   bool FreeMedia();  

private:  
	bool isPianoOn; // Flag to check if the piano is on or off.

	std::vector<TextureData> loadTextures; // List of <location, <x1, y1, w1, z1, x2, y2, w2, z2>>  
	std::vector<std::string> freeTextures; // List of locations  

	int textureProgress;
	int sceneLoaded;
};