#pragma once
#include "CollisionShape.h"
#include "Sprite.h"
#include "Vector2f.h"

class Player
{
public:
  enum class State
  {
    Idle,
    Running,
    Climbing,
    Holding,
    Dashing
  };

  // The player can move and dash in 4 cardinal directions
  enum class Direction
  {
    Left,
    Right,
    Up,
    Down,
    LeftUp,
    LeftDown,
    RightUp,
    RightDown
  };

  Player();
  ~Player();

  void Draw(bool debug = false) const;
  void Update(float elapsedSec);

  void RefillDashes(int amount);

  // Player actions
  void Jump();
  void Dash();
  void Crouch();
  void Hold();
  
  // Will move the player in a direction
  void Move(Direction direction);

  // Apply forces such as gravity and the likes
  void ApplyForce(Vector2f force);

  bool IsSliding() const;
  bool IsHolding() const;
  bool CanMove(Direction direction) const;

  const CollisionShape* GetCollisionShape() const;

  Direction GetFacingDirection() const;
  Point2f GetPosition() const;
  Vector2f GetVelocity() const;
  State GetState() const;
private:
  Vector2f m_Velocity;
  Point2f m_Position;
  int m_Dashes;

  State m_State;
  Direction m_FacingDirection;

  Sprite* m_Sprite;

  // Colliders
  CollisionShape* m_Collider;
  CollisionShape* m_ColliderFeet;
  CollisionShape* m_ColliderSlideLeft;
  CollisionShape* m_ColliderSlideRight;
};