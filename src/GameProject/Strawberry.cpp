#include "pch.h"
#include "Strawberry.h"
#include "GameDefines.h"
#include "CircleShape.h"
#include "MathUtils.h"
#include "utils.h"

Strawberry::Strawberry(const Point2f& position)
  : GameObject::GameObject(position), m_Velocity(Vector2f()), m_State(State::Idle), m_Time(0)
{
  m_SpritePtr = new Sprite(Point2f{ 18.f, 16.f }, FRAMES_PER_SECOND, STRAWBERRY_IDLE);
  m_SpritePtr->AddResource(STRAWBERRY_CONSUMING);

  m_ColliderPtr = new CircleShape(8.f * PIXEL_SCALE, m_Position + 8.f * PIXEL_SCALE, Color4f{ 0.f, 0.6f, 0.f, .5f}, true );
}

// Only the position is relevant for strawberries
Strawberry::Strawberry(const Strawberry& other)
  : GameObject::GameObject(other.GetPosition()), m_Velocity(Vector2f()), m_State(State::Idle), m_Time(0)
{
  m_SpritePtr = new Sprite(Point2f{ 18.f, 16.f }, FRAMES_PER_SECOND, STRAWBERRY_IDLE);
  m_SpritePtr->AddResource(STRAWBERRY_CONSUMING);

  m_ColliderPtr = new CircleShape(8.f * PIXEL_SCALE, m_Position + 8.f * PIXEL_SCALE, Color4f{ 0.f, 0.6f, 0.f, .5f }, true);
}

Strawberry::~Strawberry()
{
  delete m_SpritePtr;
  delete m_ColliderPtr;
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
    m_SpritePtr->SetState(0);

    if (player.GetCollisionShape()->CollidesWith(*m_ColliderPtr)) {
      m_State = State::Following;
    }

    break;
  }
  case State::Following:
  {
    m_SpritePtr->SetState(0);

    if (player.IsGrounded()) {
      m_State = State::Consuming;
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
    m_SpritePtr->SetState(1);
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
  m_ColliderPtr->SetPosition(m_Position + 8.f * PIXEL_SCALE);
}

GameObject* Strawberry::Clone() const
{
  return new Strawberry(*this); // hehe
}
