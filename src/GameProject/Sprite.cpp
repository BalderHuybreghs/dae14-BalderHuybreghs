#include "pch.h"
#include "Sprite.h"
#include "TextureManager.h"
#include <iostream>
#include "structs.h"
#include "utils.h"
#include "GameDefines.h"

using namespace utils;

Sprite::Sprite(const Point2f& frameSize, float msPerFrame, const std::string& resource)
  : m_FrameSize(frameSize), m_MsPerFrame(msPerFrame), m_Frame(0), m_Time(0), m_State({0, nullptr, 0})
{
  SetState((int)AddResource(resource) - 1, true);
}

void Sprite::Draw(const Rectf& dstRect, bool flipped, bool debug) const
{
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
  glTranslatef(dstRect.left + dstRect.width / 2, dstRect.bottom + dstRect.height / 2, 0.f);
  glScalef(flipped ? -1.f : 1.f, 1.f, 1.f);
  glTranslatef(-dstRect.left - dstRect.width / 2, -dstRect.bottom - dstRect.height / 2, 0.f);

  // Draw the sprite
  m_State.texture->Draw(dstRect, srcRect);

  glPopMatrix();
}

void Sprite::Draw(const Point2f& position, float scale, bool flipped, bool debug) const
{
  const Rectf dstRect{
    position.x,
    position.y,
    scale,
    scale
  };

  Draw(dstRect, flipped, debug);
}

void Sprite::Draw(const Point2f& position, bool flipped, bool debug) const
{
  Draw(position, 1.f, flipped, debug);
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
    (int)m_States.size(),
    texture,
    int(texture->GetWidth() / m_FrameSize.x)
  };

  m_States.push_back(state);
  return m_States.size();
}

void Sprite::SetFrame(int frame)
{
  if (frame < 0 || frame > m_State.frames) {
    std::cout << "Sprite frame for state " << m_State.id << " invalid, frame was: " << frame << std::endl;
    return;
  }

  m_Frame = frame;
}
