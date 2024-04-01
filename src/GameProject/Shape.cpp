#include "pch.h"
#include "Shape.h"
#include "utils.h"

Shape::Shape(const Point2f& position, const Color4f& color, bool filled)
  : m_Position(position), m_Color(color), m_Filled(filled)
{
}

Shape::Shape()
  : Shape(Point2f{ 0.f, 0.f })
{
}

bool Shape::ShouldTestCollisionWith(const Shape& shape) const
{
  return utils::IsOverlapping(GetBoundingBox(), shape.GetBoundingBox());
}

void Shape::SetPosition(const Point2f& position)
{
  m_Position = position;
}

void Shape::SetColor(const Color4f& color)
{
  m_Color = color;
}

void Shape::SetFilled(bool filled)
{
  m_Filled = filled;
}

Color4f Shape::GetColor() const
{
  return m_Color;
}

bool Shape::IsFilled() const
{
  return m_Filled;
}
