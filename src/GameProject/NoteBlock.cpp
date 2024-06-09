#include "pch.h"
#include "NoteBlock.h"
#include "TextureManager.h"
#include "GameDefines.h"

NoteBlock::NoteBlock(const Point2f& position, Variant variant, int width, int height)
  : GameObject(position), m_Variant(variant), m_Width(width), m_Height(height), m_Time(0)
{
  m_TexturePtr = TextureManager::GetInstance()->GetTexture(NOTE_BLOCK_FOLDER + FS + NOTE_BLOCK_SOLID);
  m_TexturePressedPtr = TextureManager::GetInstance()->GetTexture(NOTE_BLOCK_FOLDER + FS + NOTE_BLOCK_PRESSED);

  m_CollisionRect = Rectf{
    m_Position.x,
    m_Position.y,
    m_Width * 8 * PIXEL_SCALE,
    m_Height * 8 * PIXEL_SCALE
  };

  m_State = m_Variant == Variant::Blue ? State::Stable : State::Gone;
}

NoteBlock::NoteBlock(const NoteBlock& other)
  : NoteBlock(other.GetPosition(), other.GetVariant(), other.GetWidth(), other.GetHeight())
{
}

void NoteBlock::Draw(const Point2f& position, bool debug) const
{
  switch (m_State) {
    case State::Stable:
      DrawMap(m_TexturePtr, position);
      break;
    case State::Gone:
      DrawMap(m_TexturePressedPtr, position);
      break;
  }

  if (debug) {
    SetColor(Color4f{ 0.f, 0.f, 1.f, 0.8f });
    FillRect(m_CollisionRect);
  }
}

void NoteBlock::Draw(bool debug) const
{
  Draw(m_Position, debug);
}

void NoteBlock::Update(Player& player, Camera& camera, float elapsedSec)
{
  HandleCollision(player);

  if (m_Time - NOTE_BLOCK_INTERVAL >= 0.f) {
    switch (m_State) {
      case State::Stable:
        m_State = State::Gone;
        break;

      case State::Gone:
        m_State = State::Stable;
        break;
    }

    m_Time = 0.f;
  }

  m_Time += elapsedSec;
}

GameObject* NoteBlock::Clone() const
{
  return new NoteBlock(*this);
}

void NoteBlock::SetPosition(const Point2f& position)
{
  m_Position = position;

  m_CollisionRect = Rectf{
    m_Position.x,
    m_Position.y,
    m_Width * 8 * PIXEL_SCALE,
    m_Height * 8 * PIXEL_SCALE
  };
}

int NoteBlock::GetWidth() const
{
  return m_Width;
}

int NoteBlock::GetHeight() const
{
  return m_Height;
}

NoteBlock::Variant NoteBlock::GetVariant() const
{
  return m_Variant;
}

Color4f NoteBlock::GetColor() const
{
  switch (m_Variant) {
    case Variant::Blue:
      return NOTE_BLOCK_COLOR_BLUE;
    case Variant::Pink:
      return NOTE_BLOCK_COLOR_PINK;
    default:
      return Color4f{ 1.f, 1.f, 1.f, 1.f };
  }
}

bool NoteBlock::HandleCollision(Player& player) const
{
  const Rectf playerRect{ player.GetCollisionShape()->GetShape() };
  bool collided = IsOverlapping(m_CollisionRect, playerRect);

  if (collided && m_State == State::Stable) {
    Point2f position{ player.GetPosition() };
    Vector2f velocity{ player.GetVelocity() };

    // Left collision
    if (IsOverlapping(player.GetLeftCollisionRect(playerRect), m_CollisionRect)) {
      float overlap = (m_Position.x + m_CollisionRect.width) - playerRect.left;

      if (overlap > 0.f) {
        position.x += overlap; // Adjust position by the overlap amount
        velocity.x = 0;
      }
    }

    // Right collision
    if (IsOverlapping(player.GetRightCollisionRect(playerRect), m_CollisionRect)) {
      float overlap = (playerRect.left + playerRect.width) - m_Position.x;

      if (overlap > 0.f) {
        position.x -= overlap; // Adjust position by the overlap amount
        velocity.x = 0;
      }
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
      position.y -= overlap; // Adjust position by the overlap amount
      velocity.y = 0;
    }

    player.SetVelocity(velocity);
    player.SetPosition(position);
  }

  return collided;
}

void NoteBlock::DrawMap(const Texture* texture, const Point2f& position) const
{
  const Color4f color{ GetColor() };

  for (int y = 0; y < m_Height; y++)
  {
    for (int x = 0; x < m_Width; x++)
    {
      const Rectf srcRect{ GetSrcRect(x, y) };

      const Rectf dstRect{
        position.x + (8 * PIXEL_SCALE * x),
        position.y + (8 * PIXEL_SCALE * y),
        8 * PIXEL_SCALE,
        8 * PIXEL_SCALE
      };

      texture->DrawColor(dstRect, color, srcRect);
    }
  }
}

Rectf NoteBlock::GetSrcRect(int x, int y) const
{
  // Calculate the src rect
  if (x == 0 && y == m_Height -1) {
    return Rectf{
      0,
      0,
      8,
      8
    };
  }

  if (x == 0 && y == 0) {
    return Rectf{
      0,
      8 * 2,
      8,
      8
    };
  }

  if (x == m_Width -1 && y == 0) {
    return Rectf{
      8 * 2,
      8 * 2,
      8,
      8
    };
  }

  if (x == m_Width - 1 && y == m_Height - 1) {
    return Rectf{
      8 * 2,
      0,
      8,
      8
    };
  }

  if (x == 0) {
    return Rectf{
      8,
      8,
      8,
      8
    };
  }

  if (y == 0) {
    return Rectf{
      8,
      8 * 2,
      8,
      8
    };
  }

  if (y == m_Height - 1) {
    return Rectf{
      8,
      0,
      8,
      8
    };
  }

  if (x == m_Width - 1) {
    return Rectf{
      8 * 2,
      8,
      8,
      8
    };
  }

  return Rectf{
    8,
    8,
    8,
    8
  };
}
