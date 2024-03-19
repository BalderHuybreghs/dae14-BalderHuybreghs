#include "pch.h"
#include "Sprite.h"
#include "TextureManager.h"
#include <iostream>
#include "structs.h"
#include "utils.h"
#include "GameDefines.h"

using namespace utils;

Sprite::Sprite(const Point2f& position, const Point2f& size, const Point2f& frameSize, float msPerFrame, const std::string& resource)
  : m_Size(size), m_FrameSize(frameSize), m_MsPerFrame(msPerFrame), m_Frame(0), m_Time(0), m_State({nullptr, 0})
{
  SetResource(AddResource(resource) - 1);
}

void Sprite::Draw(bool debug) const
{
  const Rectf dstRect{
    m_Position.x - (m_Size.x / 2.f),
    m_Position.y + (m_Size.y / 2.f),
    m_Size.x,
    m_Size.y
  };

  if (debug) {
    DrawRect(dstRect);
  }

  // Every animation is only 1 row
  const Rectf srcRect{
    float(m_FrameSize.x * m_Frame),
    m_FrameSize.y,
    m_FrameSize.x,
    m_FrameSize.y
  };

  if (m_State.texture != nullptr) {
    m_State.texture->Draw(dstRect, srcRect);
  }
}

void Sprite::Update(float elapsedSec)
{
  m_Time += elapsedSec;

  if (m_Time >= m_MsPerFrame) {
    m_Time = 0;
    m_Frame = (m_Frame + 1) % m_State.frames;
  }
}

void Sprite::SetResource(int state)
{
  m_State = m_States.at(state);
  m_Frame = 0;
  m_Time = 0;
}

size_t Sprite::AddResource(const std::string& resource)
{
  const Texture* texture = TextureManager::Instance()->GetTexture(resource);

  const Sprite::StateInfo state{
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