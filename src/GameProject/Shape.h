#pragma once
#include "structs.h"

class Shape
{
public:
  virtual bool CollidesWith(const Shape& shape) = 0;
  virtual bool IsPointInside(const Point2f& point) = 0;

  virtual void Draw() const = 0;

  // Returns the simply x and y size of the shape, this can be used to center the shape
  virtual Rectf GetBoundingBox() const = 0;

  // Returns the center position of the shape
  virtual Point2f GetCenter() const = 0;

  // Returns a random point within the shape
  virtual Point2f GetRandomPoint() const = 0;

  // Creates a copy of the current shape, don't forget to delete the copy :)
  virtual Shape* Copy() const = 0;

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

  // Checks if shapes are close enough to test for actual collision
  bool ShouldTestCollisionWith(const Shape& shape) const;

  Point2f m_Position;
  Color4f m_Color;
  bool    m_Filled;
};