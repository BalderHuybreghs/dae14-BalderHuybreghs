#pragma once
#include "GameObject.h"
#include "Sprite.h"

class Cassette : public GameObject
{
public:
  enum class State
  {
    Idle,
    Consumed
  };

  Cassette(const Point2f& position);
  Cassette(const Cassette& other);
  ~Cassette();

  // Application of rule of 5
  Cassette(const Cassette&& other) = delete;
  Cassette& operator=(const Cassette&& other) = delete;
  Cassette& operator=(const Cassette& other) = delete;

  void Draw(const Point2f& position, bool debug) const override;
  void Draw(bool debug) const override;
  void Update(Player& player, Camera& camera, float elapsedSec) override;

  void SetPosition(const Point2f& position) override;

  GameObject* Clone() const override;
private:
  Sprite* m_SpritePtr;
  State m_State;
  Rectf m_CollisionRect;

  float m_Time;
};

