#pragma once
#include "Shape.h"
#include "Sprite.h"
#include "Vector2f.h"
#include "ParticleEmitter.h"

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

  virtual void Draw(bool debug = false) const override;
  virtual void Update(float elapsedSec) override;

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

  const Shape* GetCollisionShape() const;

  Point2f GetPosition() const;
  Vector2f GetVelocity() const;
  State GetState() const;
private:
  Vector2f m_Velocity;
  Point2f m_Position;
  int m_Dashes;

  State m_State;

  ParticleEmitter* m_JumpParticle;

  Sprite* m_Sprite;
  Sprite* m_Particle;

  // Colliders
  Shape* m_Collider;
  Shape* m_ColliderFeet;
  Shape* m_ColliderSlideLeft;
  Shape* m_ColliderSlideRight;
};