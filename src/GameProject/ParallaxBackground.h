#pragma once
#include "Texture.h"
#include <vector>

// Draws textures from front to back based on a camera position
class ParallaxBackground
{
public:
  ParallaxBackground(const std::vector<std::string>& resources);
  
  void Draw(bool debug = false);
  void Update(const Point2f& cameraPosition, float elapsedSec);

private:
  std::vector<Texture*> m_Textures;
};