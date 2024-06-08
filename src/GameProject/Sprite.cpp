#include "pch.h"
#include "Sprite.h"
#include "TextureManager.h"
#include <iostream>
#include "structs.h"
#include "utils.h"
#include "GameDefines.h"

using namespace utils;

Sprite::Sprite(const Point2f& frameSize, float msPerFrame, const std::string& state, const std::string& resource)
  : m_FrameSize(frameSize), m_MsPerFrame(msPerFrame), m_Frame(0), m_Time(0), m_State(state)
{
  AddState(state, resource);
  SetState(state, true);
}

void Sprite::Draw(const Rectf& dstRect, bool flipped, bool debug) const
{
  std::_List_const_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, const StateInfo>>>>
    state = m_States.find(m_State);

  if (debug) {
    SetColor(Color4f{ 1.f, 1.f, 1.f, 0.2f });
    FillRect(dstRect);
  }

  if (state == m_States.end()) {
    return;
  }

  // Every animation is only 1 row
  const Rectf srcRect{
    float(m_FrameSize.x * m_Frame),
    m_FrameSize.y,
    m_FrameSize.x,
    m_FrameSize.y
  };

  if (state->second.texturePtr == nullptr) {
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
  state->second.texturePtr->Draw(dstRect, srcRect);

  glPopMatrix();
}

void Sprite::Draw(const Point2f& position, float scale, bool flipped, bool debug) const
{
  const Rectf dstRect{
    position.x,
    position.y,
    m_FrameSize.x * scale,
    m_FrameSize.y * scale
  };

  Draw(dstRect, flipped, debug);
}

void Sprite::Draw(const Point2f& position, bool flipped, bool debug) const
{
  Draw(position, PIXEL_SCALE, flipped, debug);
}

void Sprite::Update(float elapsedSec)
{
  m_Time += elapsedSec;

  if (m_Time > m_MsPerFrame) {
    m_Time = 0;
    m_Frame = m_States[m_State].frames > 0 ? ((m_Frame + 1) % m_States[m_State].frames) : 0;
  }
}

void Sprite::SetState(const std::string& state, bool reset)
{
  if ((m_State == state && !reset) || !m_States.count(state)) {
    return;
  }

  m_State = state;
  m_Frame = 0;
  m_Time = 0;
}

void Sprite::AddState(const std::string& id, const std::string& resource)
{
  const Texture* texturePtr = TextureManager::GetInstance()->GetTexture(resource);

  const Sprite::StateInfo state{
    texturePtr,
    int(texturePtr->GetWidth() / m_FrameSize.x)
  };

  m_States.insert(std::make_pair(id, state));
}

Sprite::StateInfo Sprite::GetStateInfo(const std::string& id)
{
  return m_States[id]; // Lazy and dangerous implementation
}

void Sprite::SetFrame(int frame)
{
  if (frame < 0 || frame > m_States[m_State].frames) {
    std::cout << "Sprite frame for state " << m_State << " invalid, frame was: " << frame << std::endl;
    return;
  }

  m_Frame = frame;
}

bool Sprite::IsAnimationDone() const
{
  std::_List_const_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, const StateInfo>>>>
    state = m_States.find(m_State);

  if (state == m_States.end()) {
    return true;
  }

  return m_Frame >= state->second.frames - 1;
}
