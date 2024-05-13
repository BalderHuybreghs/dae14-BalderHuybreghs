#pragma once
#include "GameObject.h"

class Strawberry final : public GameObject
{
public:
  // The strawberry in Celeste has several states that it can take on depending
  // on the situation
  enum class State
  {
    Idle,
    Following,
    Consuming,
    Consumed
  };

  Strawberry(const Point2f& position);
  Strawberry(const Strawberry& other);
  ~Strawberry();

  // Application of rule of 5
  Strawberry(const Strawberry&& other) = delete;
  Strawberry& operator=(const Strawberry&& other) = delete;
  Strawberry& operator=(const Strawberry& other) = delete;

  // Inherited via GameObject
  void Draw(const Point2f& position, bool debug) const override;
  void Draw(bool debug) const override;

  void Update(Player& player, Camera& camera, float elapsedSec) override;

  void SetPosition(const Point2f& position) override;

  GameObject* Clone() const override;
private:
  // The strawberry makes use of a velocity to move towards the player
  Vector2f m_Velocity;
  State m_State;

  Sprite* m_SpritePtr;
  Shape* m_ColliderPtr;

  float m_Time;
};

