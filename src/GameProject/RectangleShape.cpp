#include "pch.h"
#include "utils.h"
#include "RectangleShape.h"
#include "CircleShape.h"

RectangleShape::RectangleShape(const Point2f& size, const Point2f& position, const Color4f& color, bool filled)
  : Shape(position, color, filled), m_Size(size)
{
}

bool RectangleShape::CollidesWith(const Shape& shape)
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

bool RectangleShape::IsPointInside(const Point2f& point)
{
  return utils::IsPointInRect(point, GetShape());
}

void RectangleShape::Draw() const
{
  utils::SetColor(m_Color);

  if (m_Filled) {
    utils::FillRect(GetShape());
  } else {
    utils::DrawRect(GetShape());
  }
}

Rectf RectangleShape::GetShape() const
{
  return Rectf(m_Position.x, m_Position.y, m_Size.x, m_Size.y);
}

Rectf RectangleShape::GetBoundingBox() const
{
    return Rectf(m_Position.x, m_Position.y, m_Size.x, m_Size.y);
}