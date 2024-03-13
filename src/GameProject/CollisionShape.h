#pragma once

class CollisionShape
{
public:
  virtual bool CollidesWith(const CollisionShape& shape) = 0;
  virtual bool IsPointInside(const Point2f& point) = 0;

  // Draw is used for debugging purposes
  // it will only be called from a debug scenario
  // hence it will not need a bool debug
  virtual void Draw() const = 0;

  void SetPosition(const Point2f& position);
  Point2f GetPosition() const;

protected:
  CollisionShape(const Point2f& position);
  CollisionShape();

  Point2f m_Position;
};