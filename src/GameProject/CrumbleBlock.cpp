#include "pch.h"
#include "CrumbleBlock.h"
#include "GameDefines.h"
#include "TextureManager.h"
#include "MathUtils.h"

CrumbleBlock::CrumbleBlock(const Point2f& position, int size, float crumbleTime, const std::string& resource)
  : GameObject(position), m_Size(size), m_CrumbleTime(crumbleTime), m_Resource(resource), m_Time(0), m_State(State::Stable)
{
  m_Texture = TextureManager::GetInstance()->GetTexture(CRUMBLE_BLOCK_FOLDER + FS + resource);
  m_OutlineTexture = TextureManager::GetInstance()->GetTexture(CRUMBLE_BLOCK_FOLDER + FS + CRUMBLE_BLOCK_OUTLINE);
}

CrumbleBlock::CrumbleBlock(const CrumbleBlock& other)
  : CrumbleBlock(other.GetPosition(), other.GetSize(), other.GetCrumbleTime(), other.GetResource())
{
}

void CrumbleBlock::Draw(const Point2f& position, bool debug) const
{
  // Create the sourcerect
  const Rectf srcRect{
    0.f,
    0.f,
    TILE_SIZE,
    TILE_SIZE
  };

  // Draw all the tiles of the crumbleblock
  for (size_t tile{ 0 }; tile < m_Size; ++tile) {
    Rectf dstRect{
      position.x + (TILE_SIZE * PIXEL_SCALE * tile),
      position.y,
      TILE_SIZE * PIXEL_SCALE,
      TILE_SIZE * PIXEL_SCALE
    };

    switch (m_State) {
    case State::Stable:
      m_Texture->Draw(dstRect, srcRect);
      break;
    case State::Unstable:
      dstRect.left   += MathUtils::RandFloat(-10.f, 10.f, 2);
      dstRect.bottom += MathUtils::RandFloat(-10.f, 10.f, 2);
      m_Texture->Draw(dstRect, srcRect);
      break;
    case State::Gone:
      m_OutlineTexture->Draw(dstRect, srcRect); // Draw only the outline if the block is gone
    }
  }
}

void CrumbleBlock::Draw(bool debug) const
{
  Draw(m_Position, debug);
}

void CrumbleBlock::Update(Player& player, Camera& camera, float elapsedSec)
{
  // Calculate the collision rectangle of this
  const Rectf collisionRect{
    m_Position.x,
    m_Position.y,
    TILE_SIZE* PIXEL_SCALE * m_Size,
    TILE_SIZE* PIXEL_SCALE
  };

  switch (m_State) {
  case State::Stable:
    if (player.IsGrounded() && IsOverlapping(collisionRect, player.GetCollisionShape()->GetShape())) {
      m_State = State::Unstable;
    }
    break;
  case State::Unstable:
    if (m_Time >= m_CrumbleTime) {
      m_State = State::Gone;
      m_Time = 0;
    }

    m_Time += elapsedSec;
    break;
  case State::Gone:
    if (m_Time >= m_CrumbleTime) {
      m_State = State::Stable;
      m_Time = 0;
    }

    m_Time += elapsedSec;
    break;
  }
}

GameObject* CrumbleBlock::Clone() const
{
  return new CrumbleBlock(*this);
}

int CrumbleBlock::GetSize() const
{
  return m_Size;
}

float CrumbleBlock::GetCrumbleTime() const
{
  return m_CrumbleTime;
}

std::string CrumbleBlock::GetResource() const
{
  return m_Resource;
}
