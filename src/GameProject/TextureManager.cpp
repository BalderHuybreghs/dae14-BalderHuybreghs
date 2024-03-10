#include "pch.h"
#include "TextureManager.h"
#include <iostream>

//using std::cout;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
  // No need to explicitly delete shared_ptr resources, they will be deleted automatically
}

void TextureManager::CleanTexture(std::string resource)
{
  auto it = m_TexturePtrs.find(resource);
  if (it == m_TexturePtrs.end()) {
    std::cout << "Clean texture not found: " << resource << std::endl;
    return;
  }

  // Check if only one reference to the texture (excluding the one in the map)
  if (it->second.use_count() == 1) {
    m_TexturePtrs.erase(it); // Remove the texture from the map
  }
}

std::shared_ptr<Texture> TextureManager::GetTexture(std::string resource)
{
  auto it = m_TexturePtrs.find(resource);
  if (it != m_TexturePtrs.end()) {
    return it->second; // Return existing shared_ptr
  }

  // Texture not found, create and insert new shared_ptr
  std::cout << "Loading texture from disk: " << resource << std::endl;
  std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(resource);
  m_TexturePtrs[resource] = newTexture;
  return newTexture;
}

TextureManager& TextureManager::GetInstance()
{
  // Thanks to https://stackoverflow.com/questions/1008019/how-do-you-implement-the-singleton-design-pattern for implementation

  static TextureManager instance;
  return instance;
}
