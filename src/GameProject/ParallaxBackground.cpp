#include "pch.h"
#include "ParallaxBackground.h"
#include "TextureManager.h"
#include "GameDefines.h"
#include <iostream>

ParallaxBackground::ParallaxBackground(const std::string& resource)
{

  TextureManager* textureManagerInstance{ TextureManager::Instance() };
  const std::string resourceFull{ BACKGROUND_FOLDER + FS + resource };

  std::cout << "Creating parallax background for '" << resourceFull << '\'' << std::endl;

  m_BackTexture = textureManagerInstance->GetTexture(resourceFull + BACKGROUND_SUFFIX_BACK);
  m_MidTexture = textureManagerInstance->GetTexture(resourceFull + BACKGROUND_SUFFIX_MID);
  m_FrontTexture = textureManagerInstance->GetTexture(resourceFull + BACKGROUND_SUFFIX_FRONT);

}

void ParallaxBackground::Draw(const Camera& camera, bool debug) const
{
  const Rectf dstRect{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

  m_BackTexture->Draw(dstRect);
  m_MidTexture->Draw(dstRect);
  m_FrontTexture->Draw(dstRect);
}
