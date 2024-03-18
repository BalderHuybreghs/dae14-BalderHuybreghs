#pragma once
#include <vector>
#include "GameObject.h"
#include "Player.h"
#include "Tilemap.h"

class Level : GameObject
{
public:
  virtual void Draw(bool debug = false) const override;
  virtual void Update(float elapsedSec) override;

  // Adds an object by reference
  void AddObject(GameObject& object);
private:
  std::vector<GameObject*> m_Objects; // All interactible objects, objects usually get a hold of the player
  Tilemap* m_Tilemap; // The tilemap for the level that contains all general blocks, the tilemap will contain collision information
};

