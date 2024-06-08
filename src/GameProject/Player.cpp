#include "pch.h"
#include "Player.h"
#include <iostream>
#include "GameDefines.h"
#include "RectangleShape.h"
#include "utils.h"
#include "MathUtils.h"
#include "structs.h"

using namespace utils;

Player::Player(const Point2f& position, const InputManager* inputManagerPtr)
  : m_State(Player::State::Idle), m_Position(position), m_Velocity(Vector2f()), m_Direction(Vector2f()), m_Dashes(1), m_ColliderPtr(nullptr),
  m_SpritePtr(nullptr), m_ParticleSpritePtr(nullptr), m_IsGrounded(false), m_Stamina(PLAYER_BASE_STAMINA), m_CanHold(false), m_Flipped(false),
  m_InputManagerPtr(inputManagerPtr)
{
  std::cout << "Creating player at (" << position.x << ", " << position.y << ')' << std::endl;

  // All the player animations
  m_SpritePtr = new Sprite(Point2f{ PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE }, FRAMES_PER_SECOND, "idle", PLAYER_IDLE_RESOURCE);
  m_SpritePtr->AddState("run", PLAYER_RUN_RESOURCE);
  m_SpritePtr->AddState("climb", PLAYER_CLIMB_RESOURCE);
  m_SpritePtr->AddState("slide", PLAYER_SLIDE_RESOURCE);
  m_SpritePtr->AddState("duck", PLAYER_DUCK_RESOURCE);
  m_SpritePtr->AddState("fall", PLAYER_FALL_RESOURCE);
  m_SpritePtr->AddState("jump", PLAYER_JUMP_RESOURCE);
  m_SpritePtr->AddState("push", PLAYER_PUSH_RESOURCE);
  m_SpritePtr->AddState("death", PLAYER_DEATH_RESOURCE);

  m_HairPtr = new Hair(m_Position, 3, HAIR_SCALE);

  const Color4f colliderColor{ 0.f, 5.f, 0.f, 0.5f };
  const Color4f colliderSideColor{ 0.f, 0.f, .5f, 0.5f };

  m_ColliderPtr = new RectangleShape(Point2f{ PLAYER_BODY_WIDTH, PLAYER_BODY_HEIGHT }, m_Position, colliderColor, true);
}

Player::~Player()
{
  std::cout << "Deleting player" << std::endl;
  delete m_SpritePtr;
  delete m_ParticleSpritePtr;
  delete m_HairPtr;
  delete m_ColliderPtr;
}

void Player::Draw(bool debug) const
{
  // Draw the hair behind the player
  const float half{ PLAYER_SCALE / 2.f };
  const bool flipped{ m_Flipped };

  m_HairPtr->Draw(flipped, debug);
  m_SpritePtr->Draw(m_Position, PIXEL_SCALE, flipped, debug);

  if (debug) {
    m_ColliderPtr->Draw();

    // Draw the circle of the actual player position value
    SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });
    FillEllipse(m_Position, 5.f, 5.f);

    // Draw the direction vector
    SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
    DrawLine(m_Position + half, m_Position + half + Vector2f(m_Direction * 10.f).ToPoint2f());

    const Rectf rect{ m_ColliderPtr->GetShape() };
    SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
    DrawRect(GetLeftCollisionRect(rect));
    DrawRect(GetRightCollisionRect(rect));

    SetColor(Color4f{ 0.f, 0.f, 1.f, 1.f });
    DrawRect(GetTopCollisionRect(rect));
    DrawRect(GetBottomCollisionRect(rect));

    SetColor(Color4f{ 0.2f, 0.3f, 0.4f, .8f });
    FillRect(GetLeftHoldRect(rect));
    FillRect(GetRightHoldRect(rect));

    SetColor(Color4f{ 0.4f, 0.4f, 0.4f, .8f });
    FillRect(GetGroundedRect(rect));
  }
}

void Player::Update(float elapsedSec, const Tilemap& tilemap)
{
  // Reset grounded, the collision handling will check if the player is grounded
  const Rectf collider{ m_ColliderPtr->GetShape() };
  m_Direction = m_Direction.Normalized();

  // Update velocity
  m_Velocity = Vector2f{
    MathUtils::Lerp(m_Velocity.x, 0.f, 0.3f),
    m_Velocity.y,
  };

  HandleCollision(elapsedSec, tilemap);
  m_IsGrounded = tilemap.IsTile(GetGroundedRect(collider));

  const float half{ PLAYER_SCALE / 2.f };
  if (floorf(m_Velocity.y) == 0.f) {
    const Point2f newPos = MathUtils::Lerp(m_HairPtr->GetPosition(), Point2f{ m_Position.x + half, m_Position.y }, 0.1f);
    m_HairPtr->SetEnd(newPos); // Slowly move the hair toward the desired position
  }

  SetPosition(Point2f{
    m_Position.x + m_Velocity.x * elapsedSec,
    m_Position.y + m_Velocity.y * elapsedSec
  });

  switch (m_State) {
    case Player::State::Idle:
    {
      if (!m_IsGrounded) {
        if (m_Velocity.y > 0.f) {
          m_SpritePtr->SetState("jump");
        } else {
          m_SpritePtr->SetState("fall");
        }
      } else if (m_InputManagerPtr->IsKeyDown(SDLK_a) || m_InputManagerPtr->IsKeyDown(SDLK_d)) {
        if (floorf(std::abs(m_Velocity.x)) <= 0.1f && m_IsGrounded) {
          m_SpritePtr->SetState("push");
        } else if (tilemap.IsTile(GetLeftHoldRect(collider)) || tilemap.IsTile(GetRightHoldRect(collider))) {
          m_State = State::Sliding;
          break;
        }
      } else if (floorf(std::abs(m_Velocity.x)) >= 0.1f) {
        m_SpritePtr->SetState("run");
      } else {
        m_SpritePtr->SetState("idle");
      }

      if (m_IsGrounded) {
        m_Dashes = 1;
        m_Stamina = PLAYER_BASE_STAMINA;
      }
      break;
    }
    case Player::State::Sliding:
    {
      if ((m_InputManagerPtr->IsKeyDown(SDLK_a) && m_Flipped) || (m_InputManagerPtr->IsKeyDown(SDLK_d) && !m_Flipped) || m_IsGrounded) {
        m_State = State::Idle;
        break;
      }

      if (!(tilemap.IsTile(GetLeftHoldRect(collider)) || tilemap.IsTile(GetRightHoldRect(collider)))) {
        m_State = State::Idle;
        break;
      }

      if (m_InputManagerPtr->IsKeyDown(SDLK_j)) {
        m_State = State::Climbing;
      }

      m_SpritePtr->SetState("slide");
      break;
    }
    case Player::State::Climbing:
    {
      if (!m_InputManagerPtr->IsKeyDown(SDLK_j) || m_Stamina <= 0.f) {
        m_State = State::Sliding;
        break;
      }

      if (m_IsGrounded) {
        m_State = State::Idle;
        break;
      }

      if (!(tilemap.IsTile(GetLeftHoldRect(collider)) || tilemap.IsTile(GetRightHoldRect(collider)))) {
        m_State = State::Idle;
        break;
      }

      if (floorf(std::abs(m_Velocity.y)) <= 0.1f) {
        m_SpritePtr->SetState("hold");
      } else {
        m_SpritePtr->SetState("climb");
      }

      m_Velocity.y = 0;
      m_Stamina -= CLIMBING_STAMINA_COST * elapsedSec;
      break;
    }
    case Player::State::Dashing:
    {
      m_SpritePtr->SetState("dash");
      if (m_SpritePtr->IsAnimationDone()) {
        m_State = State::Idle;
        break;
      }
      break;
    }
  }

  m_Flipped = (m_Flipped && m_Direction.x == 0.f) || m_Direction.x < 0.f;
  m_Direction = Vector2f();

  m_HairPtr->Update(elapsedSec);
  m_SpritePtr->Update(elapsedSec);
}

void Player::RefillDashes(int amount)
{
  if (amount < 0) {
    std::cout << "Attempt to fill dashes with negative value" << std::endl;
    return;
  }

  m_Dashes += amount;
}

void Player::Jump()
{
  if (m_IsGrounded || m_State == State::Climbing || m_State == State::Sliding) {
    m_Velocity.y = PLAYER_JUMP_FORCE;
  }
}

void Player::Dash()
{
  if (m_Dashes < 1) {
    return;
  }

  m_State = State::Dashing;
  m_Velocity = m_Direction * PLAYER_DASH_FORCE;
  --m_Dashes;
}

void Player::ApplyForce(const Vector2f& force)
{
  m_Velocity += force;
}

bool Player::CanMove(const Vector2f& direction) const
{
    return false;
}

const RectangleShape* Player::GetCollisionShape() const
{
    return m_ColliderPtr;
}

void Player::SetPosition(const Point2f& position)
{
  m_Position = position;
  const float half{ PLAYER_SCALE / 2.f };

  const Rectf colliderSize{ m_ColliderPtr->GetBoundingBox() };

  m_ColliderPtr->SetPosition(Point2f{
    m_Position.x + half - colliderSize.width / 2.f,
    m_Position.y
  });

  m_HairPtr->SetGoal(Point2f{ m_Position.x + PLAYER_HAIR_START, m_Position.y + PLAYER_HEIGHT});
}

void Player::SetVelocity(const Vector2f& velocity)
{
  m_Velocity = velocity;
}

void Player::Up()
{
  m_Direction.y = 1;

  if (m_State == State::Climbing) {
    m_Velocity.y = 100;
  }
}

void Player::Down()
{
  m_Direction.y = -1;

  if (m_State == State::Idle) {
    m_State = State::Crouching;
  } else if (m_State == State::Climbing) {
    m_Velocity.y = -100;
  }
}

void Player::Left()
{
  if (m_State == State::Climbing || m_State == State::Crouching) {
    return;
  }

  m_Direction.x = -1;

  m_Velocity.x = m_Direction.x * PLAYER_ACCELERATION; // Too lazy to type a -
}

void Player::Right()
{
  if (m_State == State::Climbing || m_State == State::Crouching) {
    return;
  }

  m_Direction.x = 1;

  m_Velocity.x = m_Direction.x * PLAYER_ACCELERATION; // Too lazy to type anything
}

Point2f Player::GetCenter() const
{
  const float half{ PLAYER_SCALE / 2.f };
  return m_Position + half;
}

Point2f Player::GetPosition() const
{
  return m_Position;
}

Vector2f Player::GetVelocity() const
{
    return m_Velocity;
}

Player::State Player::GetState() const
{
    return m_State;
}

bool Player::IsGrounded() const
{
  return m_IsGrounded;
}

void Player::Kill()
{
  m_State = State::Dead; // No matter the state, when the player is killed, they die... Who would have thought?
}

void Player::HandleCollision(float elapsedSec, const Tilemap& tilemap)
{
  const Point2f nextPlayerPos{
    GetCenter().x + m_Velocity.x * elapsedSec,
    m_Position.y + m_Velocity.y * elapsedSec
  };

  const Rectf collisionRect{
    m_ColliderPtr->GetShape()
  };

  const Rectf nextCollisionRect{
    nextPlayerPos.x - collisionRect.width / 2.f,
    nextPlayerPos.y,
    collisionRect.width,
    collisionRect.height
  };

  // Check collision with the tilemap
  if (tilemap.IsTile(nextCollisionRect))
  {
    // Handle collision response here
    // For example, stop player's movement in the direction of collision

    // Check if the collision happens on the bottom side
    if (m_Velocity.y < 0 && tilemap.IsTile(GetBottomCollisionRect(nextCollisionRect)) || tilemap.IsTile(nextPlayerPos))
    {
      // Player collided with the ground
      m_Velocity.y = 0.f;
    }

    // Check if the collision happens on the top side
    if (m_Velocity.y > 0 && tilemap.IsTile(GetTopCollisionRect(nextCollisionRect)))
    {
      // Player collided with the ceiling
      m_Velocity.y = 0.f;
    }

    // Check if the collision happens on the horizontal sides
    if (m_Velocity.x != 0 && 
        tilemap.IsTile(GetLeftCollisionRect(nextCollisionRect)) || 
        tilemap.IsTile(GetRightCollisionRect(nextCollisionRect))
        )
    {
      // Player collided with the wall
      m_Velocity.x = 0.f;
    }
  }
}

Rectf Player::GetLeftCollisionRect(const Rectf& rect) const
{
  return Rectf{ rect.left, rect.bottom + 4.f, rect.width / 4.f, rect.height - 8.f};
}

Rectf Player::GetRightCollisionRect(const Rectf& rect) const
{
  return Rectf{ rect.left + rect.width - (rect.width / 4.f), rect.bottom + 4.f, rect.width / 4.f, rect.height - 8.f };
}

Rectf Player::GetTopCollisionRect(const Rectf& rect) const
{
  return Rectf{ rect.left + 3.f, rect.bottom + rect.height - (rect.height / 4.f), rect.width - 6.f, rect.height / 4.f};
}

Rectf Player::GetBottomCollisionRect(const Rectf& rect) const
{
  return Rectf{ rect.left + 3.f, rect.bottom, rect.width - 6.f, rect.height / 4.f };
}

Rectf Player::GetLeftHoldRect(const Rectf& rect) const
{
  Rectf collisionRect{ GetLeftCollisionRect(rect) };
  collisionRect.left -= collisionRect.width;
  return collisionRect;
}

Rectf Player::GetRightHoldRect(const Rectf& rect) const
{
  Rectf collisionRect{ GetRightCollisionRect(rect) };
  collisionRect.left += collisionRect.width;
  return collisionRect;
}

Rectf Player::GetGroundedRect(const Rectf& rect) const
{
  Rectf collisionRect{ GetBottomCollisionRect(rect) };
  collisionRect.height = collisionRect.height * 0.3f;
  collisionRect.bottom -= collisionRect.height;
  return collisionRect;
}
