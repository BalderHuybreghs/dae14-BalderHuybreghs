#include "pch.h"
#include "Cassette.h"
#include "GameDefines.h"
#include "MathUtils.h"
#include "utils.h"
#include "ResourceUtils.h"

Cassette::Cassette(const Point2f& position)
  : GameObject(position), m_State(State::Idle), m_Time(0)
{
  m_SpritePtr = new Sprite(Point2f{ 32.f, 32.f }, FRAMES_PER_SECOND, "idle", CASSETTE_FOLDER + FS + CASSETTE_RESOURCE_IDLE);

  m_CollisionRect = Rectf{
    m_Position.x,
    m_Position.y,
    32.f * PIXEL_SCALE,
    32.f * PIXEL_SCALE
  };

  m_ConsumeSoundPtr = new SoundEffect(ResourceUtils::ResourceToSoundPath("cassette/consume"));
  m_ConsumeSoundPtr->SetVolume(5);
}

Cassette::Cassette(const Cassette& other)
  : Cassette(other.GetPosition())
{
}

Cassette::~Cassette()
{
  delete m_SpritePtr;
  delete m_ConsumeSoundPtr;
}

void Cassette::Draw(const Point2f& position, bool debug) const
{
  if (m_State == State::Idle) {
    m_SpritePtr->Draw(position);

    if (debug) {
      SetColor(Color4f{ 0.f, 0.f, 1.f, 0.8f });
      FillRect(m_CollisionRect);
    }
  }
}

void Cassette::Draw(bool debug) const
{
  Draw(Point2f{ m_Position.x, m_Position.y + MathUtils::Wave(15.f, 5.f, 0.f, 0.f, m_Time) }, debug);
}

void Cassette::Update(Player& player, Camera& camera, float elapsedSec)
{
  m_SpritePtr->Update(elapsedSec);

  if (m_State == State::Idle && IsOverlapping(player.GetCollisionShape()->GetShape(), m_CollisionRect)) {
    m_State = State::Consumed;
    m_ConsumeSoundPtr->Play(0);
    player.Kill(); // A lazy solution to prevent having to make the bubble thing (time constraints)
  }

  m_Time += elapsedSec;
}

void Cassette::SetPosition(const Point2f& position)
{
  m_Position = position;

  m_CollisionRect = Rectf{
    m_Position.x,
    m_Position.y,
    32.f * PIXEL_SCALE,
    32.f * PIXEL_SCALE
  };
}

GameObject* Cassette::Clone() const
{
  return new Cassette(*this);
}
