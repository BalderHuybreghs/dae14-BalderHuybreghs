#include "pch.h"
#include "utils.h"
#include "CircleShape.h"
#include "RectangleShape.h"
#include "MathUtils.h"

CircleShape::CircleShape(float radius, const Point2f& position, const Color4f& color, bool filled)
  : Shape(position, color, filled), m_Radius(radius)
{
}

CircleShape::CircleShape(const CircleShape& other)
{
  const Circlef circle{ other.GetShape() };
  m_Position = circle.center;
  m_Color = other.GetColor();
  m_Filled = other.IsFilled();
  m_Radius = circle.radius;
}

bool CircleShape::CollidesWith(const Shape& shape)
{
  if (!ShouldTestCollisionWith(shape)) {
    return false;
  }

  // Check if the other shape is a circle
  const CircleShape* otherCircle = dynamic_cast<const CircleShape*>(&shape);
  if (otherCircle)
  {
    return utils::IsOverlapping(GetShape(), otherCircle->GetShape());
  }

  // Check if the other shape is a rectangle
  const RectangleShape* otherRectangle = dynamic_cast<const RectangleShape*>(&shape);
  if (otherRectangle)
  {
    return utils::IsOverlapping(otherRectangle->GetShape(), GetShape());
  }

  return false;
}

bool CircleShape::IsPointInside(const Point2f& point)
{
  return utils::IsPointInCircle(point, GetShape());
}

void CircleShape::Draw() const
{
  utils::SetColor(m_Color);

  if (m_Filled) {
    utils::DrawEllipse(m_Position.x, m_Position.y, m_Radius, m_Radius);
  } else {
    utils::DrawEllipse(m_Position.x, m_Position.y, m_Radius, m_Radius);
  }
}

Rectf CircleShape::GetBoundingBox() const
{
  // Rectangular bounding box of a circle
  return Rectf(m_Position.x - m_Radius, m_Position.y - m_Radius, m_Radius * 2.f, m_Radius * 2.f);
}

Point2f CircleShape::GetCenter() const
{
    return m_Position;
}

Point2f CircleShape::GetRandomPoint() const
{
    return MathUtils::RandPoint(GetShape(), 2);
}

Shape* CircleShape::Copy() const
{
    return new CircleShape(*this);
}

Circlef CircleShape::GetShape() const
{
  return Circlef(m_Position.x, m_Position.y, m_Radius);
}