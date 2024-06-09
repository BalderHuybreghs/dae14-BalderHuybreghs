#include "pch.h"
#include "Jumpthru.h"
#include "GameDefines.h"
#include "TextureManager.h"

Jumpthru::Jumpthru(const Point2f& position, int size, ConnectedSides connectedSides, const std::string& resource)
  : GameObject(position), m_Size(size), m_ConnectedSides(connectedSides), m_Resource(resource)
{
  m_Texture = TextureManager::GetInstance()->GetTexture(JUMPTHRU_FOLDER + FS + resource);

  m_CollisionRect = Rectf{
    m_Position.x,
    m_Position.y,
    TILE_SIZE * PIXEL_SCALE * m_Size,
    TILE_SIZE * PIXEL_SCALE
  };
}

Jumpthru::Jumpthru(const Jumpthru& other)
  : Jumpthru(other.GetPosition(), other.GetSize(), other.GetConnectedSides(), other.GetResource())
{
}

void Jumpthru::Draw(const Point2f& position, bool debug) const
{
  // Draw all the tiles of the crumbleblock
  for (size_t tile{ 0 }; tile < m_Size; ++tile) {
    // Create the sourcerect
    Rectf srcRect{
      8,
      0,
      8,
      8
    };

    Rectf dstRect{
      position.x + (TILE_SIZE * PIXEL_SCALE * tile),
      position.y,
      TILE_SIZE * PIXEL_SCALE,
      TILE_SIZE * PIXEL_SCALE
    };

    // Determine the srcRect
    if (tile == 0) {
      switch (m_ConnectedSides) {
        case ConnectedSides::TopLeft:
        case ConnectedSides::TopBoth:
          srcRect.left = 0;
          break;
        case ConnectedSides::BottomLeft:
        case ConnectedSides::BottomBoth:
          srcRect.left = 0;
          srcRect.bottom = 8;
          break;
      }
    } else if (tile == (size_t)m_Size - 1) {
      switch (m_ConnectedSides) {
        case ConnectedSides::TopRight:
        case ConnectedSides::TopBoth:
          srcRect.left = 16;
          break;
        case ConnectedSides::BottomRight:
        case ConnectedSides::BottomBoth:
          srcRect.left = 16;
          srcRect.bottom = 8;
          break;
      }
    }

    m_Texture->Draw(dstRect, srcRect);
  }

  if (debug) {
    SetColor(Color4f{ 0.f, 0.f, 1.f, 0.8f });
    FillRect(m_CollisionRect);
  }
}

void Jumpthru::Draw(bool debug) const
{
  Draw(m_Position, debug);
}

void Jumpthru::Update(Player& player, Camera& camera, float elapsedSec)
{
  HandleCollision(player);
}

void Jumpthru::SetPosition(const Point2f& position)
{
  m_Position = position;

  m_CollisionRect = Rectf{
    m_Position.x,
    m_Position.y,
    TILE_SIZE * PIXEL_SCALE * m_Size,
    TILE_SIZE * PIXEL_SCALE
  };
}

GameObject* Jumpthru::Clone() const
{
  return new Jumpthru(*this);
}

std::string Jumpthru::GetResource() const
{
  return m_Resource;
}

Jumpthru::ConnectedSides Jumpthru::GetConnectedSides() const
{
  return m_ConnectedSides;
}

int Jumpthru::GetSize() const
{
  return m_Size;
}

bool Jumpthru::HandleCollision(Player& player) const
{
  const Rectf playerRect{ player.GetCollisionShape()->GetShape() };
  bool collided = IsOverlapping(m_CollisionRect, playerRect);

  if (collided) {
    Point2f position{ player.GetPosition() };
    Vector2f velocity{ player.GetVelocity() };

    //if (IsOverlapping(player.GetTopCollisionRect(playerRect), m_CollisionRect)) {
    //  return false;
    //}

    // Bottom collision with a smaller top collision rect
    const Rectf topCollisionRect{ m_CollisionRect.left, m_CollisionRect.bottom + m_CollisionRect.height - 2.f, m_CollisionRect.width, 2.f };
    if (IsOverlapping(player.GetBottomCollisionRect(playerRect), topCollisionRect)) {
      float overlap = (m_Position.y + m_CollisionRect.height) - playerRect.bottom;
      position.y += overlap; // Adjust position by the overlap amount

      if (velocity.y < 0) {
        velocity.y = 0;
      }

      player.SetArtificalGrounded();
    }

    if (IsOverlapping(player.GetGroundedRect(playerRect), m_CollisionRect)) {
      player.SetArtificalGrounded();
    }

    player.SetVelocity(velocity);
    player.SetPosition(position);
  }

  return collided;
}
