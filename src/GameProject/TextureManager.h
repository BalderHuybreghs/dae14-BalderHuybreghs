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
  TextureManager();
  ~TextureManager();

  // CleanTexture cleans a texture if it isn't being used by anything else
  void CleanTexture(std::string resource);
  std::shared_ptr<Texture> GetTexture(std::string resource);

  static TextureManager& GetInstance();
private:
  std::unordered_map<std::string, std::shared_ptr<Texture>> m_TexturePtrs;
};