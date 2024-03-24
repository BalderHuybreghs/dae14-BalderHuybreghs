#pragma once
#include "Sprite.h"
#include "Vector2f.h"
#include "RectangleShape.h"
#include "Tilemap.h"

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
    Falling,
    Jumping
  };

  Player(const Point2f& position);
  ~Player();

  void Draw(bool debug = false) const;

  // We may pass a list of collision objects to the players update function
  void Update(float elapsedSec, const Tilemap& tilemap);

  void RefillDashes(int amount);

  // Player actions
  void Jump();
  void Dash(const Vector2f& direction);
  void Crouch();
  void Hold();
  void LetGo();
  
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

  bool IsGrounded() const;
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

  bool m_IsGrounded;

  // Collision handling stuff
  void HandleCollision(float elapsedSec, const Tilemap& tilemap);
};