#pragma once
#include <vector>
#include "Player.h"

// The overarching class for GameObjects
// Refer to https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Virtual_Constructor for more information
// On how this idiom is used
class GameObject  
{
public:
  virtual ~GameObject() {};

  // Objects should have the ability to be drawn at any given position regadless of their own
  virtual void Draw(Point2f position, bool debug = false) const = 0;
  virtual void Draw(bool debug = false) const = 0;

  // All objects get information about the player and may manipulate the player as desired
  // This is a reverse of what professional engines tend to do, where the player (or mainly developer)
  // may query for all objects with xyz components with ECS, this approach is essentially
  // the opposite of that. All objects know of player and act accordingly.
  // This works in this context because Celeste does not have any more complex 
  virtual void Update(Player& player, float elapsedSec) = 0;

  // Allows for cloning the given gameobject
  virtual GameObject* Clone() const = 0;
 
  void SetPosition(const Point2f& position);
  Point2f GetPosition();
protected:
  GameObject(Point2f position) {};

  Point2f m_Position;
};