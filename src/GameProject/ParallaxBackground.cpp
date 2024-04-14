#include "pch.h"
#include "ParallaxBackground.h"
#include "TextureManager.h"
#include "GameDefines.h"
#include <iostream>

ParallaxBackground::ParallaxBackground(const std::string& resource)
{

  TextureManager* textureManagerInstancePtr{ TextureManager::GetInstance() };
  const std::string resourceFull{ BACKGROUND_FOLDER + FS + resource };

  std::cout << "Creating parallax background for '" << resourceFull << '\'' << std::endl;

  m_BackTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_BACK);
  m_MidTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_MID);
  m_FrontTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_FRONT);
}

void ParallaxBackground::Draw(const Camera& camera, bool debug) const
{
  const Rectf dstRect{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

  m_BackTexturePtr->Draw(dstRect);
  m_MidTexturePtr->Draw(dstRect);
  m_FrontTexturePtr->Draw(dstRect);
}
