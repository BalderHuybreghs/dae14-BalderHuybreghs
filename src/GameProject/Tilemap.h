#pragma once
#include "GameObject.h"

class Tilemap : GameObject
{
  // Inherited via GameObject
  void Draw(bool debug = false) const override;
  void Update(float elapsedSec) override;
};