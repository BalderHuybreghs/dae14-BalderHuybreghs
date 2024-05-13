#pragma once
#include "GameObject.h"
#include "Sprite.h"

class Spring final : public GameObject
{
public:
  enum class State
  {
    Idle,
    Rewinding
  };

  // The orientation of the spring, this so it can be facing the right direction depending on the tile that
  // it is placed on. I could implement a more complex system to automagically detect the orientation based
  // on the tilemap, but that just gives me more work that I don't need right now
  enum class Orientation
  {
    Left,
    Right,
    Top,
    Bottom
  };


  Spring(const Point2f& position, const Vector2f& force, Orientation orientation, float rewindTime);
  Spring(const Spring& other);
  ~Spring() override;

  // Application of rule of 5
  Spring(const Spring&& other) = delete;
  Spring& operator=(const Spring& other) = delete;
  Spring& operator=(const Spring&& other) = delete;

  // Inherited via GameObject
  void Draw(const Point2f& position, bool debug) const override;

  void Draw(bool debug) const override;

  void Update(Player& player, Camera& camera, float elapsedSec) override;

  void SetPosition(const Point2f& position) override;

  GameObject* Clone() const override;

  float GetRewindTime() const;
  Vector2f GetForce() const;
  Orientation GetOrientation() const;
private:
  float GetAngle() const;

  Sprite* m_SpritePtr;

  State m_State;
  Orientation m_Orientation;
  Vector2f m_Force; // The force the spring applies to the player

  Rectf m_Collider; // The collision rectangle for the spring

  float m_RewindTime; // Ahh yeah, it's REWIND time!
  float m_Time;
};