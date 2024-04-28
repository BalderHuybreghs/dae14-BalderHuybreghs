#include "pch.h"
#include "utils.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "MathUtils.h"

RectangleShape::RectangleShape(const Point2f& size, const Point2f& position, const Color4f& color, bool filled)
  : Shape(position, color, filled), m_Size(size)
{
}

RectangleShape::RectangleShape(const RectangleShape& other)
{
  const Rectf shape{ other.GetShape() };
  m_Color = other.GetColor();
  m_Filled = other.IsFilled();
  m_Position = Point2f{ shape.left, shape.bottom };
  m_Size = Point2f{ shape.width, shape.height };
}

bool RectangleShape::CollidesWith(const Shape& shape) const
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

bool RectangleShape::IsPointInside(const Point2f& point) const
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

Point2f RectangleShape::GetCenter() const
{
  const Rectf shape{ GetShape() };
  return Point2f{
    shape.left + shape.width / 2.f,
    shape.bottom + shape.height / 2.f
  };
}

Point2f RectangleShape::GetRandomPoint() const
{
    return MathUtils::RandPoint(GetShape(), 0);
}

Shape* RectangleShape::Copy() const
{
    return new RectangleShape(*this);
}