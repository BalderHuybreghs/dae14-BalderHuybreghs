#include "pch.h"
#include "ParallaxBackground.h"
#include "TextureManager.h"
#include "GameDefines.h"
#include "utils.h"
#include <iostream>

ParallaxBackground::ParallaxBackground(const std::string& resource, const Rectf& backRect, const Rectf& frontRect)
  : m_BackRectangle(backRect), m_FrontRectangle(frontRect)
{

  TextureManager* textureManagerInstancePtr{ TextureManager::GetInstance() };
  const std::string resourceFull{ BACKGROUND_FOLDER + FS + resource };

  std::cout << "Creating parallax background for '" << resourceFull << '\'' << std::endl;

  m_BackTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_BACK);
  m_FrontTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_FRONT);
}

ParallaxBackground::ParallaxBackground(const std::string& resource)
{
  TextureManager* textureManagerInstancePtr{ TextureManager::GetInstance() };
  const std::string resourceFull{ BACKGROUND_FOLDER + FS + resource };

  std::cout << "Creating parallax background for '" << resourceFull << '\'' << std::endl;

  m_BackTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_BACK);
  m_FrontTexturePtr = textureManagerInstancePtr->GetTexture(resourceFull + BACKGROUND_SUFFIX_FRONT);

  // Create the rectangle based on the texture sizes
  m_BackRectangle = Rectf{ 0.f, 0.f, m_BackTexturePtr->GetWidth(), m_BackTexturePtr->GetHeight() };
  m_FrontRectangle = Rectf{ 0.f, 0.f, m_FrontTexturePtr->GetWidth(), m_FrontTexturePtr->GetHeight() };
}

void ParallaxBackground::Draw(const Camera& camera, bool debug) const
{
  const Point2f cameraPosition{ camera.GetPosition() };

  const Point2f backPosition{
    cameraPosition.x - m_BackRectangle.left,
    cameraPosition.y - m_BackRectangle.bottom,
  };

  const Point2f frontPosition{
    cameraPosition.x - m_FrontRectangle.left,
    cameraPosition.y - m_FrontRectangle.bottom,
  };

  // Calculate the progression percentage
  const Point2f backProgress{
    backPosition.x / (m_BackRectangle.width  - WINDOW_WIDTH),
    backPosition.y / (m_BackRectangle.height - WINDOW_HEIGHT)
  };

  const Point2f frontProgress{
    frontPosition.x / (m_FrontRectangle.width  - WINDOW_WIDTH),
    frontPosition.y / (m_FrontRectangle.height - WINDOW_HEIGHT)
  };

  // To move the textures only very little
  const float scale{ 1 - BACKGROUND_DOWNSCALE };

  const Point2f backTextureScale{
    m_BackTexturePtr->GetWidth()  * scale,
    m_BackTexturePtr->GetHeight() * scale
  };

  const Point2f frontTextureScale{
    m_FrontTexturePtr->GetWidth()  * scale,
    m_FrontTexturePtr->GetHeight() * scale
  };

  // The amount the position needs to move
  const Point2f backTextureMove{
    m_BackTexturePtr->GetWidth()  - backTextureScale.x,
    m_BackTexturePtr->GetHeight() - backTextureScale.y,
  };

  const Point2f frontTextureMove{
    m_FrontTexturePtr->GetWidth()  - frontTextureScale.x,
    m_FrontTexturePtr->GetHeight() - frontTextureScale.y,
  };

  const Rectf srcRectBack{ 
    backProgress.x * backTextureMove.x, 
    (1 - frontProgress.y) * backProgress.y * backTextureMove.y,
    backTextureScale.x, 
    backTextureScale.y
  };

  const Rectf srcRectFront{ 
    frontProgress.x * frontTextureMove.x,
    (1 - frontProgress.y) * frontTextureMove.y,
    frontTextureScale.x,
    frontTextureScale.y
  };

  const Rectf dstRect{ 0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT};

  m_BackTexturePtr->Draw(dstRect, srcRectBack);
  m_FrontTexturePtr->Draw(dstRect, srcRectFront);

  if (!debug) {
    return;
  }

  // Hacky debug thingamabob
  camera.PushMatrix();
  utils::SetColor(Color4f{0.f, 0.4f, 0.f, 1.f});
  utils::DrawRect(m_BackRectangle, 4.f);
  utils::DrawRect(m_FrontRectangle, 4.f);
  camera.PopMatrix();
}

Rectf ParallaxBackground::GetMidRect() const
{
    return m_BackRectangle;
}

Rectf ParallaxBackground::GetFrontRect() const
{
    return m_FrontRectangle;
}

void ParallaxBackground::SetMidRect(const Rectf& rect)
{
  m_BackRectangle = rect;
}

void ParallaxBackground::SetFrontRect(const Rectf& rect)
{
  m_FrontRectangle = rect;
}
