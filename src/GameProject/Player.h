#pragma once
#include "Sprite.h"
#include "Vector2f.h"
#include "RectangleShape.h"
#include "Tilemap.h"
#include "Hair.h"

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
    Jumping,
    Pushing
  };

  Player(const Point2f& position);
  ~Player();

  void Draw(bool debug = false) const;

  // We may pass a list of collision objects to the players update function
  void Update(float elapsedSec, const Tilemap& tilemap);

  void RefillDashes(int amount);

  // Player actions
  void Jump();
  void Dash();
  void Hold();

  // Movement functions the player can do
  void Up();
  void Down();
  void Left();
  void Right();

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
  Vector2f m_Direction;
  Vector2f m_Velocity;
  Point2f m_Position;
  int m_Dashes;

  // The player Celeste has stamina
  // source: https://celeste.ink/wiki/Stamina
  float m_Stamina;

  State m_State;

  Sprite* m_Sprite;

  // Visual effects stuff
  Sprite* m_Particle;

  Hair* m_Hair; // The hair rendered behind the player

  // Colliders
  RectangleShape* m_Collider;
  RectangleShape* m_ColliderFeet;
  RectangleShape* m_ColliderLeft;
  RectangleShape* m_ColliderRight;

  bool m_IsGrounded;
  bool m_CanHold;

  // Collision handling stuff
  void HandleCollision(float elapsedSec, const Tilemap& tilemap);
};