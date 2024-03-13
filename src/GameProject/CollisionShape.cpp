#include "pch.h"
#include "CollisionShape.h"

CollisionShape::CollisionShape(const Point2f& position)
  : m_Position(position)
{
}

CollisionShape::CollisionShape()
  : CollisionShape(Point2f{ 0.f, 0.f })
{
}

void CollisionShape::SetPosition(const Point2f& position)
{
  m_Position = position;
}

Point2f CollisionShape::GetPosition() const
{
  return m_Position;
}
