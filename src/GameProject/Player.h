#pragma once
#include "Sprite.h"
#include "Vector2f.h"
#include "RectangleShape.h"
#include "Tilemap.h"
#include "Hair.h"
#include "InputManager.h"

class Player final
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
    Pushing,
    Dead
  };

  Player(const Point2f& position, const InputManager* inputManagerPtr);

  // Look, I know that I am deleting a lot of copy operators / constructors, but I barely use these anyway and I lack the
  // time to actually implement something which I currently do not use. I'll implement them when I actually need them
  Player(const Player& other) = delete;
  Player& operator=(const Player& other) = delete;
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

  Point2f GetCenter() const;
  Point2f GetPosition() const;
  Vector2f GetVelocity() const;
  State GetState() const;

  bool IsGrounded() const;

  void Kill(); // Instantly kills the player (as Celeste does not work with a health / damage system)
private:
  // Collision handling stuff
  void HandleCollision(float elapsedSec, const Tilemap& tilemap);

  Vector2f m_Direction;
  Vector2f m_Velocity;
  Point2f m_Position;
  int m_Dashes;

  // The player Celeste has stamina
  // source: https://celeste.ink/wiki/Stamina
  float m_Stamina;

  State m_State;

  Sprite* m_SpritePtr;

  // Visual effects stuff
  Sprite* m_ParticleSpritePtr;

  Hair* m_HairPtr; // The hair rendered behind the player

  // Colliders
  RectangleShape* m_ColliderPtr;
  RectangleShape* m_ColliderLeftPtr;
  RectangleShape* m_ColliderRightPtr;

  bool m_IsGrounded;
  bool m_CanHold;
  bool m_Flipped;

  // To handle the input live we use the input manager here
  // this is so we can easily check if the player is holding buttons
  // to solve wonky controls
  const InputManager* m_InputManagerPtr;
};