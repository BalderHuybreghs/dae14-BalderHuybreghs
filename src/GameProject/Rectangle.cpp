#include "pch.h"
#include "Rectangle.h"
#include "Circle.h"
#include "utils.h"

Rectangle::Rectangle(const Point2f& size, const Point2f& position, const Color4f& color = Color4f{ 0.f, 0.f, 0.f, 1.f }, bool filled = false)
  : Shape(position, color, filled), m_Size(size)
{
}

bool Rectangle::CollidesWith(const Shape& shape)
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

bool Rectangle::IsPointInside(const Point2f& point)
{
  return utils::IsPointInRect(point, GetShape());
}

void Rectangle::Draw() const
{
  utils::SetColor(m_Color);

  if (m_Filled) {
    utils::FillRect(GetShape());
  } else {
    utils::DrawRect(GetShape());
  }
}

Rectf Rectangle::GetShape() const
{
  return Rectf(m_Position.x, m_Position.y, m_Size.x, m_Size.y);
}
