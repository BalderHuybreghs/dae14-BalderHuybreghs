#pragma once
#include "GameObject.h"
#include "Texture.h"

class Decal : public GameObject
{
public:
  Decal(Texture* texturePtr);

  // Inherited via GameObject
  void Draw(bool debug = false) const override;
  void Update(Player& player, Camera& camera, float elapsedSec) override;

private:
  // Inherited via GameObject
  void Draw(const Point2f& position, bool debug) const override;
  GameObject* Clone() const override;

  Texture* m_TexturePtr;
};