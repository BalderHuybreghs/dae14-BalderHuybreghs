#pragma once
#include "Shape.h"

class CircleShape : public Shape
{
public:
  CircleShape(float radius, const Point2f& position = Point2f(0.0f, 0.0f), const Color4f& color = Color4f{ 0.f, 0.f, 0.f, 1.f }, bool filled = false);

  virtual bool CollidesWith(const Shape& shape) override;

  virtual bool IsPointInside(const Point2f& point) override;

  virtual void Draw() const override;

  Circlef GetShape() const;
private:
  float m_Radius;

  // Inherited via Shape
  Point2f GetSimpleSize() const override;
};