#pragma once
#include "Sprite.h"
#include "Vector2f.h"
#include "RectangleShape.h"

class Player
{
public:
  enum class State
  {
    Idle,
    Running,
    Climbing,
    Holding,
    Dashing,
    Sliding,
    Crouching,
    Falling
  };

  Player(const Point2f& position);
  ~Player();

  void Draw(bool debug = false) const;
  void Update(float elapsedSec);

  void RefillDashes(int amount);

  // Player actions
  void Jump();
  void Dash(const Vector2f& direction);
  void Crouch();
  void Hold();
  
  // Will move the player in a direction
  void Move(const Vector2f& direction);

  // Apply forces such as gravity and the likes
  void ApplyForce(const Vector2f& force);

  bool CanMove(const Vector2f& direction) const;

  const RectangleShape* GetCollisionShape() const;

  void SetPosition(const Point2f& position);
  void SetVelocity(const Vector2f& velocity);

  Point2f GetPosition() const;
  Vector2f GetVelocity() const;
  State GetState() const;
private:
  Vector2f m_Velocity;
  Point2f m_Position;
  int m_Dashes;

  State m_State;

  Sprite* m_Sprite;
  Sprite* m_Particle;

  // Colliders
  RectangleShape* m_Collider;
  RectangleShape* m_ColliderFeet;
  RectangleShape* m_ColliderSlideLeft;
  RectangleShape* m_ColliderSlideRight;
};