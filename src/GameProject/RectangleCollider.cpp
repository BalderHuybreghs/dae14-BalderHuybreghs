#include "pch.h"
#include "RectangleCollider.h"
#include "CircleCollider.h"
#include "utils.h"
#include "CollisionShape.h"
#include "RectangleCollider.h"

RectangleCollider::RectangleCollider(const Point2f& size, const Point2f& position)
  : m_Size(size)
{
}

bool RectangleCollider::CollidesWith(const CollisionShape& shape)
{
  // Check if the other shape is a circle
  const CircleCollider* otherCircle = dynamic_cast<const CircleCollider*>(&shape);
  if (otherCircle)
  {
    return utils::IsOverlapping(GetShape(), otherCircle->GetShape());
  }

  // Check if the other shape is a rectangle
  const RectangleCollider* otherRectangle = dynamic_cast<const RectangleCollider*>(&shape);
  if (otherRectangle)
  {
    return utils::IsOverlapping(otherRectangle->GetShape(), GetShape());
  }

  return false;
}

bool RectangleCollider::IsPointInside(const Point2f& point)
{
  return utils::IsPointInRect(point, GetShape());
}

void RectangleCollider::Draw() const
{
  utils::DrawRect(GetShape());
}

Rectf RectangleCollider::GetShape() const
{
  return Rectf(m_Position.x, m_Position.y, m_Size.x, m_Size.y);
}
