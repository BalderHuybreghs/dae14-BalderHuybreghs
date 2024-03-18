#pragma once
#include <vector>
#include "Player.h"

// The overarching class for GameObjects, 
class GameObject  
{
public:
  virtual ~GameObject() {};

  virtual void Draw(bool debug = false) const = 0;

  // All objects get information about the player and may manipulate the player as desired
  // This is a reverse of what professional engines tend to do, where the player (or mainly developer)
  // may query for all objects with xyz components with ECS, this approach is essentially
  // the opposite of that. All objects know of player and act accordingly.
  // This works in this context because Celeste does not have any more complex 
  virtual void Update(Player& player, float elapsedSec) = 0;
protected:
  GameObject() {};
};