#include "pch.h"
#include "CircleCollider.h"
#include "utils.h"
#include "CollisionShape.h"
#include "RectangleCollider.h"

CircleCollider::CircleCollider(float radius, const Point2f& position)
  : CollisionShape(position), m_Radius(radius)
{
}

bool CircleCollider::CollidesWith(const CollisionShape& shape)
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

bool CircleCollider::IsPointInside(const Point2f& point)
{
  return utils::IsPointInCircle(point, GetShape());
}

void CircleCollider::Draw() const
{
  utils::DrawEllipse(m_Position.x, m_Position.y, m_Radius, m_Radius);
}

Circlef CircleCollider::GetShape() const
{
  return Circlef(m_Position.x, m_Position.y, m_Radius);
}
