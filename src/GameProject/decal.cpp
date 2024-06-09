#include "pch.h"
#include "Decal.h"
#include "GameDefines.h"

Decal::Decal(const Point2f& position, const std::string& resource, const Point2f& frameSize)
  : GameObject(position), m_Resource(resource), m_FrameSize(frameSize)
{
  m_SpritePtr = new Sprite(m_FrameSize, FRAMES_PER_SECOND, "idle", DECAL_FOLDER + FS + resource);
}

Decal::Decal(const Decal& other)
  : Decal(other.GetPosition(), other.m_Resource, other.m_FrameSize)
{
}

Decal::~Decal()
{
  delete m_SpritePtr;
}

void Decal::Draw(const Point2f& position, bool debug) const
{
  m_SpritePtr->Draw(position, false, debug);
}

void Decal::Draw(bool debug) const
{
  Draw(m_Position, debug);
}

void Decal::Update(Player& player, Camera& camera, float elapsedSec)
{
  m_SpritePtr->Update(elapsedSec);
}


GameObject* Decal::Clone() const
{
  return new Decal(*this);
}
