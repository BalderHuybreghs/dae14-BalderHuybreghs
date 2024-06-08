#include "pch.h"
#include "Spike.h"
#include "GameDefines.h"
#include "TextureManager.h"

Spike::Spike(const Point2f& position, Orientation orientation)
  : GameObject(position), m_Orientation(orientation)
{
  std::string resource{};

  switch (orientation) {
    case Orientation::Left:
      resource = SPIKE_LEFT;
      break;
    case Orientation::Right:
      resource = SPIKE_RIGHT;
      break;
    case Orientation::Top:
      resource = SPIKE_UP;
      break;
    case Orientation::Bottom:
      resource = SPIKE_DOWN;
      break;
  }

  m_TexturePtr = TextureManager::GetInstance()->GetTexture(DANGER_FOLDER + FS + resource);

  m_Collider = Rectf{
    m_Position.x,
    m_Position.y,
    m_TexturePtr->GetWidth() * PIXEL_SCALE,
    m_TexturePtr->GetHeight() * PIXEL_SCALE
  };
}

Spike::Spike(const Spike& other)
  : Spike(other.GetPosition(), other.GetOrientation())
{
}

void Spike::Draw(const Point2f& position, bool debug) const
{
  Rectf dstRect{
    position.x,
    position.y,
    TILE_SIZE * PIXEL_SCALE,
    TILE_SIZE * PIXEL_SCALE
  };

  m_TexturePtr->Draw(dstRect);
}

void Spike::Draw(bool debug) const
{
  Draw(m_Position, debug);
}

void Spike::Update(Player& player, Camera& camera, float elapsedSec)
{
  // If player toucha my spaghet, player equals dead
  if (IsOverlapping(m_Collider, player.GetCollisionShape()->GetShape())){
    player.Kill();
  }
}

void Spike::SetPosition(const Point2f& position)
{
  m_Position = position;
  m_Collider = Rectf{
    m_Position.x,
    m_Position.y,
    m_TexturePtr->GetWidth() * PIXEL_SCALE,
    m_TexturePtr->GetHeight() * PIXEL_SCALE
  };
}

Spike::Orientation Spike::GetOrientation() const
{
  return m_Orientation;
}

GameObject* Spike::Clone() const
{
  return new Spike(*this);
}
