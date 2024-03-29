#include "pch.h"
#include "Sprite.h"
#include "TextureManager.h"
#include <iostream>
#include "structs.h"
#include "utils.h"
#include "GameDefines.h"

using namespace utils;

Sprite::Sprite(const Point2f& position, const Point2f& size, const Point2f& frameSize, float msPerFrame, const std::string& resource)
  : m_Position(position), m_Size(size), m_FrameSize(frameSize), m_MsPerFrame(msPerFrame), m_Frame(0), m_Time(0), m_State({0, nullptr, 0}), m_Mirror(false)
{
  SetState(AddResource(resource) - 1, true);
}

void Sprite::Draw(bool debug) const
{
  Rectf dstRect{
    m_Position.x,
    m_Position.y,
    m_Size.x,
    m_Size.y
  };

  if (debug) {
    SetColor(Color4f{ 1.f, 1.f, 1.f, 0.2f });
    FillRect(dstRect);
  }

  // Every animation is only 1 row
  const Rectf srcRect{
    float(m_FrameSize.x * m_Frame),
    m_FrameSize.y,
    m_FrameSize.x,
    m_FrameSize.y
  };

  if (m_State.texture == nullptr) {
    std::cout << "Sprite texture is null" << std::endl;
    return;
  }

  // For mirroring the sprite
  glPushMatrix();

  // Apply mirroring transformation
  glTranslatef(m_Position.x + m_Size.x / 2, m_Position.y + m_Size.y / 2, 0.f);
  glScalef(m_Mirror ? -1 : 1, 1, 1);
  glTranslatef(-m_Position.x - m_Size.x / 2, -m_Position.y - m_Size.y / 2, 0.f);

  // Draw the sprite
  m_State.texture->Draw(dstRect, srcRect);

  glPopMatrix();
}

void Sprite::Update(float elapsedSec)
{
  m_Time += elapsedSec;

  if (m_Time > m_MsPerFrame) {
    m_Time = 0;
    m_Frame = (m_Frame + 1) % m_State.frames;
  }
}

void Sprite::SetState(int state, bool reset)
{
  if (m_State.id == state && !reset) {
    return;
  }

  m_State = m_States.at(state);
  m_Frame = 0;
  m_Time = 0;
}

size_t Sprite::AddResource(const std::string& resource)
{
  const Texture* texture = TextureManager::Instance()->GetTexture(resource);

  const Sprite::StateInfo state{
    m_States.size(),
    texture,
    int(texture->GetWidth() / m_FrameSize.x)
  };

  m_States.push_back(state);
  return m_States.size();
}

Point2f Sprite::GetPosition() const
{
  return m_Position;
}

Point2f Sprite::GetSize() const
{
  return m_Size;
}

void Sprite::SetPosition(const Point2f& position)
{
  m_Position = position;
}

void Sprite::SetSize(const Point2f& size)
{
  m_Size = size;
}

void Sprite::SetMirror(bool mirror)
{
  m_Mirror = mirror;
}
