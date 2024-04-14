#pragma once
#include "Texture.h"
#include "Camera.h"
#include <vector>

// Draws textures from front to back based on a camera position
class ParallaxBackground
{
public:
  ParallaxBackground(const std::string& resource); // Loads both a foreground AND a background texture
  
  void Draw(const Camera& camera, bool debug = false) const;
private:
  const Texture* m_BackTexturePtr;
  const Texture* m_MidTexturePtr;
  const Texture* m_FrontTexturePtr;
};