#include "pch.h"
#include "CheckPoint.h"
#include "utils.h"
#include "GameDefines.h"

CheckPoint::CheckPoint(const Point2f& position)
  : GameObject(position)
{
  m_CollisionRect = Rectf{
    position.x,
    position.y,
    64 * PIXEL_SCALE,
    64 * PIXEL_SCALE
  };
}

CheckPoint::CheckPoint(const CheckPoint& other)
  : CheckPoint(other.GetPosition())
{
}

void CheckPoint::Draw(const Point2f& position, bool debug) const
{
  if (debug) {
    SetColor(Color4f{ .5f, .5f, .5f, .5f });
    FillRect(m_CollisionRect);
  }
}

void CheckPoint::Draw(bool debug) const
{
  Draw(m_Position, debug);
}

void CheckPoint::Update(Player& player, Camera& camera, float elapsedSec)
{
  if (IsOverlapping(player.GetCollisionShape()->GetShape(), m_CollisionRect)) {
    Point2f point{ m_CollisionRect.Center() };
    point.y = m_CollisionRect.bottom;
    player.SetRespawnPoint(point);
  }
}

GameObject* CheckPoint::Clone() const
{
  return new CheckPoint(*this);
}

void CheckPoint::SetPosition(const Point2f& position)
{
  m_Position = position;

  m_CollisionRect = Rectf{
    position.x,
    position.y,
    64 * PIXEL_SCALE,
    64 * PIXEL_SCALE
  };
}
