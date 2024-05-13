#include "pch.h"
#include "CrumbleBlock.h"
#include "GameDefines.h"
#include "TextureManager.h"
#include "MathUtils.h"

CrumbleBlock::CrumbleBlock(const Point2f& position, int size, float crumbleTime, const std::string& resource)
  : GameObject(position), m_Size(size), m_CrumbleTime(crumbleTime), m_Resource(resource), m_Time(0), m_State(State::Stable), m_PlayerPosBeforeCollision(Point2f{})
{
  m_TexturePtr = TextureManager::GetInstance()->GetTexture(CRUMBLE_BLOCK_FOLDER + FS + resource);
  m_OutlineTexturePtr = TextureManager::GetInstance()->GetTexture(CRUMBLE_BLOCK_FOLDER + FS + CRUMBLE_BLOCK_OUTLINE);

  // Calculate the collision rect based on the size
  m_CollisionRect = Rectf{
      m_Position.x,
      m_Position.y,
      TILE_SIZE * PIXEL_SCALE * m_Size,
      TILE_SIZE * PIXEL_SCALE
  };

}

CrumbleBlock::CrumbleBlock(const CrumbleBlock& other)
  : CrumbleBlock(other.GetPosition(), other.GetSize(), other.GetCrumbleTime(), other.GetResource())
{
}

void CrumbleBlock::Draw(const Point2f& position, bool debug) const
{
  // Draw all the tiles of the crumbleblock
  for (size_t tile{ 0 }; tile < m_Size; ++tile) {
    // Create the sourcerect
    const Rectf srcRect{
      tile > 0 ? TILE_SIZE : (tile == m_Size -1 ? TILE_SIZE * 2 : 0.f),
      0.f,
      TILE_SIZE,
      TILE_SIZE
    };

    Rectf dstRect{
      position.x + (TILE_SIZE * PIXEL_SCALE * tile),
      position.y,
      TILE_SIZE * PIXEL_SCALE,
      TILE_SIZE * PIXEL_SCALE
    };

    switch (m_State) {
    case State::Stable:
      m_TexturePtr->Draw(dstRect, srcRect);
      break;
    case State::Unstable:
      dstRect.left   += MathUtils::RandFloat(-10.f, 10.f, 2);
      dstRect.bottom += MathUtils::RandFloat(-10.f, 10.f, 2);
      m_TexturePtr->Draw(dstRect, srcRect);
      break;
    case State::Gone:
      m_OutlineTexturePtr->Draw(dstRect, srcRect); // Draw only the outline if the block is gone
    }
  }
}

void CrumbleBlock::Draw(bool debug) const
{
  Draw(m_Position, debug);
}

void CrumbleBlock::Update(Player& player, Camera& camera, float elapsedSec)
{
  bool collided{ HandleCollision(player) };

  switch (m_State) {
  case State::Stable:
    if (collided && player.GetPosition().y >= m_CollisionRect.bottom + m_CollisionRect.height) {
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

bool CrumbleBlock::HandleCollision(Player& player)
{
  const Rectf playerRect = player.GetCollisionShape()->GetShape();
  const Rectf blockRect = m_CollisionRect;
  const Point2f playerPosition = player.GetPosition();
  const Vector2f playerVelocity = player.GetVelocity();

  bool collided = IsOverlapping(blockRect, playerRect);

  if (collided && m_State != State::Gone) {
    // If the player is not colliding with the left or right side of the block
    if (playerPosition.x + playerRect.width > blockRect.left && playerPosition.x < blockRect.left + blockRect.width) {
      if (playerVelocity.y < 0) {
        // Player moving upwards, adjust player position to be just above the block
        player.SetPosition(Point2f(playerPosition.x, blockRect.bottom));
        // Stop player's upward velocity
        player.SetVelocity(Vector2f(playerVelocity.x, 0));
      } else if (playerVelocity.y > 0) {
        // Player moving downwards, adjust player position to be just below the block
        player.SetPosition(Point2f(playerPosition.x, blockRect.bottom - playerRect.height));
        // Stop player's downward velocity
        player.SetVelocity(Vector2f(playerVelocity.x, 0));
      }
    } else {
      // If the player is colliding with the bottom or top side of the block
      if (playerVelocity.x < 0) {
        // Player moving left, adjust player position to be just to the left of the block
        player.SetPosition(Point2f(blockRect.left - playerRect.width, playerPosition.y));
        // Stop player's leftward velocity
        player.SetVelocity(Vector2f(0, playerVelocity.y));
      } else if (playerVelocity.x > 0) {
        // Player moving right, adjust player position to be just to the right of the block
        player.SetPosition(Point2f(blockRect.left + blockRect.width, playerPosition.y));
        // Stop player's rightward velocity
        player.SetVelocity(Vector2f(0, playerVelocity.y));
      }
    }
  }

  return collided;
}

