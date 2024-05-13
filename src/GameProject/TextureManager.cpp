#include "pch.h"
#include "TextureManager.h"
#include <iostream>
#include "GameDefines.h"
#include "ResourceUtils.h"

TextureManager* TextureManager::instance = nullptr;

TextureManager::~TextureManager()
{
  std::cout << "Cleaning registered textures" << std::endl;

  // Delete all the textures
  for (const std::pair<std::string, const Texture*>& pair : m_TexturePtrs) {
    std::cout << "Deleting texture with ID: '" << pair.first << '\'' << std::endl;
    delete pair.second;
  }

  // Clear out the hashmap
  m_TexturePtrs.clear();
}

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
  // No auto allowed :/
  std::pair<std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, const Texture*>>>>, bool> success{ m_TexturePtrs.insert(
    {
      resource,
      CreateTextureInstance(resource)
    }
  ) };

  if (success.second) {
    return success.first->second;
  }

  std::cerr << "Failed to load texture: " << resource << std::endl;
  exit(-1);
}

const Texture* TextureManager::GetTexture(const std::string& resource)
{
  std::unordered_map<std::string, const Texture*>::iterator value{ m_TexturePtrs.find(resource) };
  if (value != m_TexturePtrs.end()) {
    return value->second; // Return existing shared_ptr
  }

  // Texture not found, create and insert new shared_ptr
  std::cout << "Loading texture from disk: " << resource << std::endl;
  return CreateFromResource(resource);
}

TextureManager* TextureManager::GetInstance()
{
  if (instance == nullptr) {
    std::cout << "Creating texture manager" << std::endl;
    instance = new TextureManager();
  }

  return instance;
}

void TextureManager::DestroyInstance()
{
  std::cout << "Destroying texture manager" << std::endl;
  delete instance;
  instance = nullptr;
}
