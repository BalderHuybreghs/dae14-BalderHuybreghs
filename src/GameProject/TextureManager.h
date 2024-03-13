#pragma once
#include "Texture.h"
#include <unordered_map>
#include <memory>

// The texture manager will manage all textures in a memory efficient way.
// The idea is that there will be a single instance of this class in the entire program
// making use of the singleton pattern. 
class TextureManager
{
public:
  ~TextureManager();

  // CleanTexture cleans a texture if it isn't being used by anything else
  const Texture* GetTexture(const std::string& resource);

  static TextureManager* Instance();
protected:
  const Texture* CreateTextureInstance(const std::string& resource);
  const Texture* CreateFromResource(const std::string& resource);
private:
  std::unordered_map<std::string, const Texture*> m_TexturePtrs;

  static TextureManager* _instance;
};