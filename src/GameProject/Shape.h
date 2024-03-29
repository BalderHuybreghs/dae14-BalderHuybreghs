#pragma once
#include "structs.h"

class Shape
{
public:
  virtual bool CollidesWith(const Shape& shape) = 0;
  virtual bool IsPointInside(const Point2f& point) = 0;

  virtual void Draw() const = 0;

  // Returns the simply x and y size of the shape, this can be used to center the shape
  virtual Point2f GetSimpleSize() const = 0;

  // Atempts to lerp one shape into the other, if unimplemented or unfinished,
  // will just write to console and return itself
  // virtual Shape Lerp(const Shape& other, float t) = 0;
  
  Point2f GetPosition() const;
  Color4f GetColor() const;
  bool    IsFilled() const;

  void SetPosition(const Point2f& position);
  void SetColor(const Color4f& color);
  void SetFilled(bool filled);
protected:
  Shape(const Point2f& position, const Color4f& color = Color4f{ 0.f, 0.f, 0.f, 1.f }, bool filled = false);
  Shape();

  Point2f m_Position;
  Color4f m_Color;
  bool    m_Filled;
};