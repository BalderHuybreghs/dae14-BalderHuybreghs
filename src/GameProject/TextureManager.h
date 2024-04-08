#pragma once
#include "Texture.h"
#include <unordered_map>
#include <memory>

// The texture manager will manage all textures in a memory efficient way.
// The idea is that there will be a single instance of this class in the entire program
// making use of the singleton pattern. 
class TextureManager final
{
public:
  // Default constructor
  TextureManager() = default;

  // Prevent making copies of this class, application of rule of 3
  TextureManager(const TextureManager& other) = delete;
  TextureManager& operator=(const TextureManager& other) = delete;
  ~TextureManager();

  const Texture* GetTexture(const std::string& resource);

  static TextureManager* Instance();
  static void DestroyInstance();
protected:
  const Texture* CreateTextureInstance(const std::string& resource);
  const Texture* CreateFromResource(const std::string& resource);
private:
  std::unordered_map<std::string, const Texture*> m_TexturePtrs;

  static TextureManager* _instance;
};