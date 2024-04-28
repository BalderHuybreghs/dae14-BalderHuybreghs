#pragma once
#include "Shape.h"

class CircleShape : public Shape
{
public:
  CircleShape(float radius, const Point2f& position = Point2f(0.0f, 0.0f), const Color4f& color = Color4f{ 0.f, 0.f, 0.f, 1.f }, bool filled = false);
  
  CircleShape(const CircleShape& other);
  CircleShape& operator=(const CircleShape& other) = delete;
  virtual ~CircleShape() override = default;

  virtual bool CollidesWith(const Shape& shape) const override;

  virtual bool IsPointInside(const Point2f& point) const override;

  virtual void Draw() const override;
  
  virtual Rectf GetBoundingBox() const override;

  virtual Point2f GetCenter() const override;

  virtual Point2f GetRandomPoint() const override;

  virtual Shape* Copy() const override;

  Circlef GetShape() const;
private:
  float m_Radius;
};