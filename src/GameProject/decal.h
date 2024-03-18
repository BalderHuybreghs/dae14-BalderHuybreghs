#pragma once
#include "GameObject.h"
#include "Texture.h"

class Decal : public GameObject
{
public:
  Decal(Texture* texture);

  // Inherited via GameObject
  void Draw(bool debug = false) const override;
  void Update(Player& player, float elapsedSec) override;

private:
  Texture* m_Texture;
};