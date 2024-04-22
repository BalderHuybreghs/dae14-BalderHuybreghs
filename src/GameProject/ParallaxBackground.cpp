#include "pch.h"
#include "ParallaxBackground.h"
#include "TextureManager.h"
#include "GameDefines.h"
#include "utils.h"
#include <iostream>

ParallaxBackground::ParallaxBackground(const std::string& resource, const Rectf& midRect, const Rectf& frontRect)
  : m_MidRectangle(midRect), m_FrontRectangle(frontRect)
{

  TextureManager* textureManagerInstancePtr{ TextureManager::GetInstance() };
  const std::string resourceFull{ BACKGROUND_FOLDER + FS + resource };

  std::cout << "Creating parallax background for '" << resourceFull << '\'' << std::endl;

  m_BackTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_BACK);
  m_MidTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_MID);
  m_FrontTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_FRONT);
}

ParallaxBackground::ParallaxBackground(const std::string& resource)
{
  TextureManager* textureManagerInstancePtr{ TextureManager::GetInstance() };
  const std::string resourceFull{ BACKGROUND_FOLDER + FS + resource };

  std::cout << "Creating parallax background for '" << resourceFull << '\'' << std::endl;

  m_BackTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_BACK);
  m_MidTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_MID);
  m_FrontTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_FRONT);

  // Create the rectangle based on the texture sizes
  m_MidRectangle = Rectf{ 0.f, 0.f, m_MidTexturePtr->GetWidth(), m_MidTexturePtr->GetHeight() };
  m_FrontRectangle = Rectf{ 0.f, 0.f, m_FrontTexturePtr->GetWidth(), m_FrontTexturePtr->GetHeight() };
}

void ParallaxBackground::Draw(const Camera& camera, bool debug) const
{
  const Point2f cameraPosition{ camera.GetPosition() };

  const Point2f midPosition{
    cameraPosition.x - m_MidRectangle.left,
    cameraPosition.y - m_MidRectangle.bottom,
  };

  const Point2f frontPosition{
    cameraPosition.x - m_FrontRectangle.left,
    cameraPosition.y - m_FrontRectangle.bottom,
  };

  // Calculate the progression percentage
  const Point2f midProgress{
    midPosition.x / (m_MidRectangle.width - WINDOW_WIDTH),
    midPosition.y / (m_MidRectangle.height - WINDOW_HEIGHT)
  };

  const Point2f frontProgress{
    frontPosition.x / (m_FrontRectangle.width - WINDOW_WIDTH),
    frontPosition.y / (m_FrontRectangle.height - WINDOW_HEIGHT)
  };

  const Rectf dstRectBack{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
  const Rectf dstRectMid{ midProgress.x * m_MidTexturePtr->GetWidth(), midProgress.y * m_MidTexturePtr->GetHeight(), WINDOW_WIDTH, WINDOW_HEIGHT};
  const Rectf dstRectFront{ frontProgress.x * m_FrontTexturePtr->GetWidth(), frontProgress.y * m_FrontTexturePtr->GetHeight(), WINDOW_WIDTH, WINDOW_HEIGHT };

  m_BackTexturePtr->Draw(dstRectBack);
  m_MidTexturePtr->Draw(dstRectMid);
  m_FrontTexturePtr->Draw(dstRectFront);

  if (!debug) {
    return;
  }

  // Hacky debug thingamabob
  camera.PushMatrix();
  utils::SetColor(Color4f{0.f, 0.f, 1.f, 1.f});
  utils::DrawRect(m_MidRectangle);
  utils::DrawRect(m_FrontRectangle);
  camera.PopMatrix();
}

Rectf ParallaxBackground::GetMidRect() const
{
    return m_MidRectangle;
}

Rectf ParallaxBackground::GetFrontRect() const
{
    return m_FrontRectangle;
}

void ParallaxBackground::SetMidRect(const Rectf& rect)
{
  m_MidRectangle = rect;
}

void ParallaxBackground::SetFrontRect(const Rectf& rect)
{
  m_FrontRectangle = rect;
}
