#pragma once
#include "GameObject.h"

class DashRefill final : public GameObject
{
public:
  // The strawberry in Celeste has several states that it can take on depending
  // on the situation
  enum class State
  {
    Idle,
    Consumed
  };

  DashRefill(const Point2f& position);
  DashRefill(const DashRefill& other);
  ~DashRefill() override;

  // Application of rule of 5
  DashRefill(const DashRefill&& other) = delete;
  DashRefill& operator=(const DashRefill& other) = delete;
  DashRefill& operator=(const DashRefill&& other) = delete;

  // Inherited via GameObject
  void Draw(const Point2f& position, bool debug) const override;
  void Draw(bool debug) const override;

  void Update(Player& player, Camera& camera, float elapsedSec) override;

  void SetPosition(const Point2f& position) override;

  GameObject* Clone() const override;
private:
  State m_State;

  Sprite* m_SpritePtr;
  Shape* m_ColliderPtr;

  float m_Cooldown; // The time it takes to refill the dash
  float m_Time;
};

