#pragma once
#include <vector>

// The overarching class for GameObjects, 
class GameObject  
{
public:
  virtual void Draw(bool debug = false) const = 0;
  virtual void Update(float elapsedSec) = 0;
protected:
  GameObject() {};
};