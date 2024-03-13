#pragma once
#include "CollisionShape.h"

class RectangleCollider : public CollisionShape
{
public:
  RectangleCollider(const Point2f& size, const Point2f& position = Point2f(0.0f, 0.0f));

  virtual bool CollidesWith(const CollisionShape& shape) override;

  virtual bool IsPointInside(const Point2f& point) override;

  virtual void Draw() const override;

  Rectf GetShape() const;
private:
  Point2f m_Size;
};
