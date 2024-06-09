#include "pch.h"
#include "DashRefill.h"
#include "CircleShape.h"
#include "GameDefines.h"
#include "ResourceUtils.h"

DashRefill::DashRefill(const Point2f& position)
  : GameObject::GameObject(position), m_State(State::Idle), m_Time(0), m_Cooldown(0)
{
  m_SpritePtr = new Sprite(Point2f{ 16.f, 16.f }, FRAMES_PER_SECOND, "idle", REFILL_IDLE);
  m_SpritePtr->AddState("outline", REFILL_OUTLINE);

  m_ColliderPtr = new CircleShape(8.f * PIXEL_SCALE, m_Position + 8.f * PIXEL_SCALE, Color4f{ 0.f, 0.6f, 0.f, .5f }, true);

  m_TouchSoundPtr = new SoundEffect(ResourceUtils::ResourceToSoundPath("refill/touch"));
  m_TouchSoundPtr->SetVolume(5);

  m_ReturnSoundPtr = new SoundEffect(ResourceUtils::ResourceToSoundPath("refill/return"));
  m_ReturnSoundPtr->SetVolume(5);
}

DashRefill::DashRefill(const DashRefill& other)
  : DashRefill(other.GetPosition())
{
}

DashRefill::~DashRefill()
{
  delete m_SpritePtr;
  delete m_ColliderPtr;
  delete m_TouchSoundPtr;
  delete m_ReturnSoundPtr;
}

void DashRefill::Draw(const Point2f& position, bool debug) const
{
  m_SpritePtr->Draw(position, false, debug);

  if (debug) {
    m_ColliderPtr->Draw();
  }
}

void DashRefill::Draw(bool debug) const
{
  Point2f position{};

  switch (m_State) {
    case State::Idle:
      // TODO, define these wave values as a global const sometime
      position = Point2f{ m_Position.x, m_Position.y + MathUtils::Wave(15.f, 5.f, 0.f, 0.f, m_Time) };
      break;
    case State::Consumed:
      position = m_Position;
      break;
  }

  Draw(position, debug); // Draw with a height offset based on a sine wave
}

void DashRefill::Update(Player& player, Camera& camera, float elapsedSec)
{
  switch (m_State) {
  case State::Idle:
  {
    m_SpritePtr->SetState("idle");

    if (player.GetCollisionShape()->CollidesWith(*m_ColliderPtr)) {
      m_TouchSoundPtr->Play(0);
      m_State = State::Consumed;
      player.RefillDashes(1);
      player.ResetStamina();
      player.SetDashCooldown(0.01f); // Protect the player from accidentally spending their dash immediately
      m_Cooldown = 2.5f; // A 2.5 second cooldown, refer to https://celeste.ink/wiki/Dash_Refills
    }

    m_Time += elapsedSec;
    break;
  }
  case State::Consumed:
  {
    m_SpritePtr->SetState("outline"); // Set the sprite state to the consumed sprite

    if (m_Cooldown <= 0.f) {
      m_ReturnSoundPtr->Play(0);
      m_State = State::Idle; 
    }

    m_Cooldown -= elapsedSec; // Keep track of the cooldown
    m_Time = 0;
    break;
  }
  }

  m_SpritePtr->Update(elapsedSec);
}

void DashRefill::SetPosition(const Point2f& position)
{
  m_Position = position;
  m_ColliderPtr->SetPosition(m_Position + 8.f * PIXEL_SCALE);
}

GameObject* DashRefill::Clone() const
{
  return new DashRefill(*this);
}
