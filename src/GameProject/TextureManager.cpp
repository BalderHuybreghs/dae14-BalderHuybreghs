#include "pch.h"
#include "TextureManager.h"
#include <iostream>
#include "GameDefines.h"
#include "ResourceUtils.h"

TextureManager* TextureManager::_instance = nullptr;

const Texture* TextureManager::CreateTextureInstance(const std::string& resource)
{
  // Create the texture
  const std::string path{ ResourceUtils::ResourceToImagePath(resource) };
  Texture* texturePtr{ new Texture(path) };

  // Check if the texture was able to be created, if not, the texture will be a default one
  if (texturePtr->IsCreationOk()) {
    return texturePtr;
  }

  std::cerr << "Texture not found: " << path << std::endl;

  delete texturePtr;
  texturePtr = new Texture(MISSING_TEXTURE_TEXTURE);

  if (!texturePtr->IsCreationOk()) {
    std::cerr << "Missing texture is missing, aborting program" << std::endl;
    exit(-1);
  }

  return texturePtr;
}

const Texture* TextureManager::CreateFromResource(const std::string& resource)
{
  auto success{ m_TexturePtrs.insert(
    {
      resource,
      CreateTextureInstance(resource)
    }
  ) };

  if (success.second) {
    return success.first->second;
  }

  std::cerr << "Failed to load texture: " << resource << std::endl;
}

TextureManager::~TextureManager()
{
  // Delete all the textures
  for (auto& pair : m_TexturePtrs) {
    delete pair.second;
    pair.second = nullptr;
  }

  // Clear out the hashmap
  m_TexturePtrs.clear();
}

const Texture* TextureManager::GetTexture(const std::string& resource)
{
  auto value{ m_TexturePtrs.find(resource) };
  if (value != m_TexturePtrs.end()) {
    return value->second; // Return existing shared_ptr
  }

  // Texture not found, create and insert new shared_ptr
  std::cout << "Loading texture from disk: " << resource << std::endl;
  return CreateFromResource(resource);
}

TextureManager* TextureManager::Instance()
{
  if (_instance == nullptr) {
    _instance = new TextureManager();
  }

  return _instance;
}