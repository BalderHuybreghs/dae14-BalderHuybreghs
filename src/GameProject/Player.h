#pragma once
#include "Shape.h"
#include "Sprite.h"
#include "Vector2f.h"
#include "GameObject.h"

class Player : GameObject
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

  virtual void Draw(bool debug = false) const override;
  virtual void Update(float elapsedSec) override;

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

  const Shape* GetCollisionShape() const;

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
  Shape* m_Collider;
  Shape* m_ColliderFeet;
  Shape* m_ColliderSlideLeft;
  Shape* m_ColliderSlideRight;
};