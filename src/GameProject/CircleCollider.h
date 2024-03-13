#pragma once
#include "CollisionShape.h"

class CircleCollider : public CollisionShape
{
public:
  CircleCollider(float radius, const Point2f& position = Point2f(0.0f, 0.0f));

  virtual bool CollidesWith(const CollisionShape& shape) override;

  virtual bool IsPointInside(const Point2f& point) override;

  virtual void Draw() const override;

  Circlef GetShape() const;
private:
  float m_Radius;
};
