#pragma once
#include "Shape.h"

class Rectangle : public Shape
{
public:
  Rectangle(const Point2f& size, const Point2f& position = Point2f(0.0f, 0.0f), const Color4f& color = Color4f{ 0.f, 0.f, 0.f, 1.f }, bool filled = false);

  virtual bool CollidesWith(const Shape& shape) override;

  virtual bool IsPointInside(const Point2f& point) override;

  virtual void Draw() const override;

  Rectf GetShape() const;
private:
  Point2f m_Size;
};