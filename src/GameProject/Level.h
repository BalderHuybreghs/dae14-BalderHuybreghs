#pragma once
#include <vector>
#include "GameObject.h"

class Level : GameObject
{
public:
  
  virtual void Draw(bool debug = false) const override;
  virtual void Update(float elapsedSec) override;

  // Adds an object by reference
  void AddObject(GameObject& object);
private:
  std::vector<GameObject*> m_Objects;
};

