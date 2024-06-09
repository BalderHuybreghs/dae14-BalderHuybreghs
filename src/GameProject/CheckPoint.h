#pragma once
#include "GameObject.h"

class CheckPoint : public GameObject
{
public:
  CheckPoint(const Point2f& position);
  CheckPoint(const CheckPoint& other);

  // Rule of 5
  ~CheckPoint() = default;
  CheckPoint(const CheckPoint&& other) = delete;
  CheckPoint& operator=(const CheckPoint& other) = delete;
  CheckPoint& operator=(const CheckPoint&& other) = delete;

  void Draw(const Point2f& position, bool debug) const override;
  void Draw(bool debug) const override;
  void Update(Player& player, Camera& camera, float elapsedSec) override;
  GameObject* Clone() const override;

  void SetPosition(const Point2f& position) override;
private:
  Rectf m_CollisionRect;
};

