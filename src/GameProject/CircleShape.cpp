#include "pch.h"
#include "utils.h"
#include "CircleShape.h"
#include "RectangleShape.h"

CircleShape::CircleShape(float radius, const Point2f& position, const Color4f& color, bool filled)
  : Shape(position, color, filled), m_Radius(radius)
{
}

bool CircleShape::CollidesWith(const Shape& shape)
{
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

Circlef CircleShape::GetShape() const
{
  return Circlef(m_Position.x, m_Position.y, m_Radius);
}

Point2f CircleShape::GetSimpleSize() const
{
  return Point2f(m_Radius, m_Radius);
}