#pragma once
#include "Shape.h"

class RectangleShape : public Shape
{
public:
  RectangleShape(const Point2f& size, const Point2f& position = Point2f(0.0f, 0.0f), const Color4f& color = Color4f{ 0.f, 0.f, 0.f, 1.f }, bool filled = false);

  virtual bool CollidesWith(const Shape& shape) override;

  virtual bool IsPointInside(const Point2f& point) override;

  virtual void Draw() const override;

  virtual Rectf GetBoundingBox() const override;

  virtual Point2f GetCenter() const override;

  virtual Point2f GetRandomPoint() const override;

  virtual Shape* Copy() const override;

  Rectf GetShape() const;
private:
  Point2f m_Size;
};