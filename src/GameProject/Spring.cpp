#include "pch.h"
#include "GameDefines.h"
#include "Spring.h"
#include "TextureManager.h"
#include "utils.h"

using namespace utils;

Spring::Spring(const Point2f& position, const Vector2f& force, Orientation orientation, float rewindTime)
  : GameObject(position), m_Force(force), m_Orientation(orientation), m_RewindTime(rewindTime), m_Time(0), m_State(State::Idle)
{
  // Create the collider, the values are based on the texture
  m_Collider = Rectf{
    m_Position.x + 2 * PIXEL_SCALE,
    m_Position.y,
    12 * PIXEL_SCALE,
    3 * PIXEL_SCALE
  };

  m_SpritePtr = new Sprite(Point2f{ 16, 16 }, rewindTime / 5.f, "idle", SPRING_IDLE);
  m_SpritePtr->AddState("push", SPRING_PUSH);

  m_JumpSoundPtr = new SoundEffect(SOUND_FOLDER + FS + "spring.ogg");
  m_JumpSoundPtr->SetVolume(10);
}

Spring::Spring(const Spring& other)
  : Spring(other.GetPosition(), other.GetForce(), other.GetOrientation(), other.GetRewindTime())
{
}

Spring::~Spring()
{
  delete m_SpritePtr;

  m_JumpSoundPtr->StopAll();
  delete m_JumpSoundPtr;
}

void Spring::Draw(const Point2f& position, bool debug) const
{
  const float centerOffset{ 8 * PIXEL_SCALE };

  glPushMatrix();
  glTranslatef(position.x + centerOffset, position.y + centerOffset, 0.f);

  glRotatef(GetAngle(), 0.f, 0.f, 1.f);

  glTranslatef(-(position.x + centerOffset), -(position.y + centerOffset), 0.f);

  m_SpritePtr->Draw(position, false, debug);
  glPopMatrix();

  if (!debug) {
    return;
  }

  SetColor(Color4f{ 1.f, 1.f, 0.f, .5f });
  FillRect(m_Collider);
}

void Spring::Draw(bool debug) const
{
  Draw(m_Position, debug);
}

void Spring::Update(Player& player, Camera& camera, float elapsedSec)
{
  switch (m_State) {
  case (State::Idle):
    {
      m_SpritePtr->SetState("idle");

      if (IsOverlapping(m_Collider, player.GetCollisionShape()->GetShape())) {
        m_State = State::Rewinding;
        player.SetVelocity(player.IsHoldingSpace() ? m_Force * 1.3f : m_Force);
        m_SpritePtr->SetState("push");
        m_Time = 0.f;

        m_JumpSoundPtr->Play(0);
      }

      break;
    }
  case (State::Rewinding):
    {
      if (m_SpritePtr->IsAnimationDone()) {
        m_SpritePtr->SetState("idle"); // Reset the animation state
      }

      if (m_Time >= m_RewindTime) {
        m_State = State::Idle;
        m_Time = 0.f;
      }

      m_Time += elapsedSec;
      break;
    }
  }

  m_SpritePtr->Update(elapsedSec);
}

void Spring::SetPosition(const Point2f& position)
{
  m_Position = position;
  m_Collider.left = m_Position.x + 2 * PIXEL_SCALE;
  m_Collider.bottom = m_Position.y;
}

GameObject* Spring::Clone() const
{
    return new Spring(*this);
}

float Spring::GetRewindTime() const
{
  return m_RewindTime;
}

Vector2f Spring::GetForce() const
{
  return m_Force;
}

Spring::Orientation Spring::GetOrientation() const
{
  return m_Orientation;
}

float Spring::GetAngle() const
{
  // Get the rotation angle in degrees based on the orientation :)
  switch (m_Orientation) {
  case Orientation::Bottom:
    return 0.f;
  case Orientation::Right:
    return 90.f;
  case Orientation::Top:
    return 180.f;
  case Orientation::Left:
    return 270.f;
  default:
    return 0.f;
  }
}
