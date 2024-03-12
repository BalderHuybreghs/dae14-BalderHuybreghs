#pragma once
#include "Texture.h"
#include <unordered_map>
#include <memory> 
#include "Resource.h"

// The texture manager will manage all textures in a memory efficient way.
// The idea is that there will be a single instance of this class in the entire program
// making use of the singleton pattern. 
class TextureManager
{
public:
  // CleanTexture cleans a texture if it isn't being used by anything else
  const Texture* GetTexture(const Resource& resource);

  static TextureManager* Instance();
protected:
  const Texture* CreateTextureInstance(const Resource& resource);
  const Texture* CreateFromResource(const Resource& resource);
private:
  std::unordered_map<std::string, const Texture*> m_TexturePtrs;

  static TextureManager* _instance;
};