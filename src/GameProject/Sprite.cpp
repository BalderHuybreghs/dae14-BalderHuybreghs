#include "pch.h"
#include "Sprite.h"
#include "TextureManager.h"
#include <iostream>
#include "structs.h"
#include "utils.h"
#include "GameDefines.h"

using namespace utils;

Sprite::Sprite(const Point2f& size)
  : m_Frame(0), m_Size(size), m_Time(0), m_State({ nullptr, 0 })
{
}

Sprite::Sprite(const Point2f& size, const std::string& resource)
  : Sprite(size)
{
  SetResource(AddResource(resource) - 1);
}

void Sprite::Draw(const Point2f& position, bool debug) const
{
  const Rectf dstRect{
    position.x - (m_Size.x / 2.f),
    position.y + (m_Size.y / 2.f),
    m_Size.x,
    m_Size.y
  };

  if (debug) {
    DrawRect(dstRect);
  }

  // Every animation is only 1 row
  const Rectf srcRect{
    float(FRAME_SIZE * m_Frame),
    FRAME_SIZE,
    FRAME_SIZE,
    FRAME_SIZE
  };

  if (m_State.texture != nullptr) {
    m_State.texture->Draw(dstRect, srcRect);
  }
}

void Sprite::Update(float elapsedTime)
{
  m_Time += elapsedTime;

  if (m_Time >= FRAMES_PER_SECOND) {
    m_Time = 0;
  }
}

void Sprite::SetResource(int state)
{
  m_State = m_States.at(state);
  m_Frame = 0;
  m_Time = 0;
}

int Sprite::AddResource(const std::string& resource)
{
  const Texture* texture = TextureManager::Instance()->GetTexture(resource);

  const Sprite::StateInfo state{
    texture,
    int(texture->GetWidth() / FRAME_SIZE)
  };

  m_States.push_back(state);
  return m_States.size();
}

Point2f Sprite::GetSize() const
{
  return m_Size;
}

void Sprite::SetSize(const Point2f& size)
{
  m_Size = size;
}
