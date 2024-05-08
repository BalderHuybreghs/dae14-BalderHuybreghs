#pragma once
#include "GameObject.h"

class Spring final : public GameObject
{
public:

  // Inherited via GameObject
  void Draw(const Point2f& position, bool debug) const override;

  void Draw(bool debug) const override;

  void Update(Player& player, Camera& camera, float elapsedSec) override;

  GameObject* Clone() const override;

private:

};