#include "pch.h"
#include "CrumbleBlock.h"
#include "GameDefines.h"
#include "TextureManager.h"
#include "MathUtils.h"
#include "ResourceUtils.h"

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

  m_BreakSoundEffectPtr = new SoundEffect(ResourceUtils::ResourceToSoundPath("break"));
  m_BreakSoundEffectPtr->SetVolume(5);
}

CrumbleBlock::CrumbleBlock(const CrumbleBlock& other)
  : CrumbleBlock(other.GetPosition(), other.GetSize(), other.GetCrumbleTime(), other.GetResource())
{
}

CrumbleBlock::~CrumbleBlock()
{
  delete m_BreakSoundEffectPtr;
}

void CrumbleBlock::Draw(const Point2f& position, bool debug) const
{
  // Draw all the tiles of the crumbleblock
  for (size_t tile{ 0 }; tile < m_Size; ++tile) {
    // Create the sourcerect
    const Rectf srcRect{
      tile > 0 ? (tile == m_Size - 1 ? TILE_SIZE * 2 : TILE_SIZE) : (m_Size == 1 ? TILE_SIZE * 3 : 0.f),
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

  if (debug) {
    SetColor(Color4f{ 0.f, 0.f, 1.f, 0.8f });
    FillRect(m_CollisionRect);
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
      m_BreakSoundEffectPtr->Play(0);
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

void CrumbleBlock::SetPosition(const Point2f& position)
{
  m_Position = position;

  m_CollisionRect = Rectf{
    m_Position.x,
    m_Position.y,
    TILE_SIZE * PIXEL_SCALE * m_Size,
    TILE_SIZE * PIXEL_SCALE
  };
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

bool CrumbleBlock::HandleCollision(Player& player) const
{
  const Rectf playerRect{ player.GetCollisionShape()->GetShape() };
  bool collided = IsOverlapping(m_CollisionRect, playerRect);

  if (collided && m_State != State::Gone) {
    Point2f position{ player.GetPosition() };
    Vector2f velocity{ player.GetVelocity() };

    // Left collision
    if (IsOverlapping(player.GetLeftCollisionRect(playerRect), m_CollisionRect)) {
      float overlap = (m_Position.x + m_CollisionRect.width) - playerRect.left;
      position.x += overlap; // Adjust position by the overlap amount
      velocity.x = 0;
    }

    // Right collision
    if (IsOverlapping(player.GetRightCollisionRect(playerRect), m_CollisionRect)) {
      float overlap = (playerRect.left + playerRect.width) - m_Position.x;
      position.x -= overlap; // Adjust position by the overlap amount
      velocity.x = 0;
    }

    // Bottom collision
    if (IsOverlapping(player.GetBottomCollisionRect(playerRect), m_CollisionRect)) {
      float overlap = (m_Position.y + m_CollisionRect.height) - playerRect.bottom;
      position.y += overlap; // Adjust position by the overlap amount
      velocity.y = 0;
      player.SetArtificalGrounded();
    }

    if (IsOverlapping(player.GetGroundedRect(playerRect), m_CollisionRect)) {
      player.SetArtificalGrounded();
    }

    // Top collision
    if (IsOverlapping(player.GetTopCollisionRect(playerRect), m_CollisionRect)) {
      float overlap = (playerRect.bottom + playerRect.height) - m_Position.y;
      position.y += overlap; // Adjust position by the overlap amount
      velocity.y = 0;
    }

    player.SetVelocity(velocity);
    player.SetPosition(position);
  }

  return collided;
}
