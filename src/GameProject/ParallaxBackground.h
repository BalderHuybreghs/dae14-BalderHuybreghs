#pragma once
#include "Texture.h"
#include "Camera.h"
#include <vector>

// Draws textures from front to back based on a camera position
class ParallaxBackground
{
public:
  ParallaxBackground(const std::string& resource, const Rectf& backRect, const Rectf& frontRect); // Loads both a foreground AND a background texture
  ParallaxBackground(const std::string& resource);

  void Draw(const Camera& camera, bool debug = false) const;

  Rectf GetMidRect() const;
  Rectf GetFrontRect() const;

  void SetMidRect(const Rectf& rect);
  void SetFrontRect(const Rectf& rect);
private:
  // Rectangles for relativistic background movement
  Rectf m_BackRectangle;
  Rectf m_FrontRectangle;

  const Texture* m_BackTexturePtr;
  const Texture* m_FrontTexturePtr;
};