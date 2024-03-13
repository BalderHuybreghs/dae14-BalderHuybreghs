#include "pch.h"
#include "Circle.h"
#include "utils.h"
#include "RectangleCollider.h"

Circle::Circle(float radius, const Point2f& position, const Color4f& color = Color4f{ 0.f, 0.f, 0.f, 1.f }, bool filled = false)
  : Shape(position, color, filled), m_Radius(radius)
{
}

bool Circle::CollidesWith(const Shape& shape)
{
  // Check if the other shape is a circle
  const Circle* otherCircle = dynamic_cast<const Circle*>(&shape);
  if (otherCircle)
  {
    return utils::IsOverlapping(GetShape(), otherCircle->GetShape());
  }

  // Check if the other shape is a rectangle
  const Rectangle* otherRectangle = dynamic_cast<const Rectangle*>(&shape);
  if (otherRectangle)
  {
    return utils::IsOverlapping(otherRectangle->GetShape(), GetShape());
  }

  return false;
}

bool Circle::IsPointInside(const Point2f& point)
{
  return utils::IsPointInCircle(point, GetShape());
}

void Circle::Draw() const
{
  utils::SetColor(m_Color);

  if (m_Filled) {
    utils::DrawEllipse(m_Position.x, m_Position.y, m_Radius, m_Radius);
  } else {
    utils::DrawEllipse(m_Position.x, m_Position.y, m_Radius, m_Radius);
  }
}

Circlef Circle::GetShape() const
{
  return Circlef(m_Position.x, m_Position.y, m_Radius);
}
