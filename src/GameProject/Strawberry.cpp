#include "pch.h"
#include "Strawberry.h"
#include "GameDefines.h"
#include "CircleShape.h"
#include "MathUtils.h"
#include "utils.h"

Strawberry::Strawberry(const Point2f& position)
  : GameObject(position), m_Velocity(Vector2f()), m_State(State::Idle), m_Time(0), m_StartPos(position)
{
  m_SpritePtr = new Sprite(Point2f{ 18.f, 16.f }, FRAMES_PER_SECOND, "idle", STRAWBERRY_IDLE);
  m_SpritePtr->AddState("consume", STRAWBERRY_CONSUMING);

  m_ColliderPtr = new CircleShape(8.f * PIXEL_SCALE, m_Position + 8.f * PIXEL_SCALE, Color4f{ 0.f, 0.6f, 0.f, .5f}, true);

  m_ConsumeSoundPtr = new SoundEffect(STRAWBERRY_SOUND_FOLDER + FS + "consume.ogg");
  m_TouchSoundPtr = new SoundEffect(STRAWBERRY_SOUND_FOLDER + FS + "touch.ogg");

  m_ConsumeSoundPtr->SetVolume(10);
  m_TouchSoundPtr->SetVolume(10);
}

// Only the position is relevant for strawberries
Strawberry::Strawberry(const Strawberry& other)
  : Strawberry(other.GetPosition())
{
}

Strawberry::~Strawberry()
{
  delete m_SpritePtr;
  delete m_ColliderPtr;

  m_ConsumeSoundPtr->StopAll();
  m_TouchSoundPtr->StopAll();

  delete m_ConsumeSoundPtr;
  delete m_TouchSoundPtr;
}

void Strawberry::Draw(const Point2f& position, bool debug) const
{
  // Draw the strawberry if it has not yet been consumed
  switch (m_State) {
    case State::Consumed: 
      break;
    default:
    {
      m_SpritePtr->Draw(position, false, debug);

      if (debug) {
        m_ColliderPtr->Draw();
      }
      break;
    }
  }
}

void Strawberry::Draw(bool debug) const
{
  Draw(Point2f{ m_Position.x, m_Position.y + MathUtils::Wave(15.f, 5.f, 0.f, 0.f, m_Time)}, debug); // Draw with a height offset based on a sine wave
}

void Strawberry::Update(Player& player, Camera& camera, float elapsedSec)
{
  m_Position += m_Velocity * elapsedSec;

  switch (m_State) {
  case State::Idle:
  {
    m_SpritePtr->SetState("idle");

    if (player.GetCollisionShape()->CollidesWith(*m_ColliderPtr)) {
      m_State = State::Following;
      m_TouchSoundPtr->Play(0);
    }

    break;
  }
  case State::Following:
  {
    m_SpritePtr->SetState("idle");

    if (player.GetState() == Player::State::Dead) {
      m_State = State::Idle;
      m_Position = m_StartPos;
      m_Velocity = Vector2f{};
      break;
    }

    if (player.IsGrounded()) {
      m_State = State::Consuming;
      m_ConsumeSoundPtr->Play(0);
      break;
    }

    // Move towards the top of the player
    const Rectf playerShape{ player.GetCollisionShape()->GetShape() };
    const Point2f playerPos{ (playerShape.left + playerShape.width / 2.f) - 8.f * PIXEL_SCALE, playerShape.bottom + playerShape.height };
    const Point2f diff{ playerPos.x - m_Position.x, playerPos.y - m_Position.y };

    // Calculate the trajectory
    const float distance{ sqrtf(powf(diff.x, 2) + powf(diff.y, 2)) };
    const float direction{ atan2f(diff.y, diff.x) };

    const Vector2f velocity{
      cos(direction) * distance * 5.f,
      sin(direction) * distance * 5.f
    };

    m_Velocity = velocity; // Move the berry towards the player
    break;
  }

  case State::Consuming:
  {
    m_SpritePtr->SetState("consume");
    m_Velocity = Vector2f{}; // Reset the velocity

    if (m_SpritePtr->IsAnimationDone()) {
      m_State = State::Consumed;
    }

    break;
  }
  }

  m_SpritePtr->Update(elapsedSec);
  m_Time += elapsedSec;
}

void Strawberry::SetPosition(const Point2f& position)
{
  m_Position = position;
  m_StartPos = m_Position;
  m_ColliderPtr->SetPosition(m_Position + 8.f * PIXEL_SCALE);
}

GameObject* Strawberry::Clone() const
{
  return new Strawberry(*this); // hehe
}
