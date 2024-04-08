#include "pch.h"
#include "Player.h"
#include <iostream>
#include "GameDefines.h"
#include "RectangleShape.h"
#include "utils.h"
#include "MathUtils.h"
#include "structs.h"

using namespace utils;

Player::Player(const Point2f& position)
  : m_State(Player::State::Idle), m_Position(position), m_Velocity(Vector2f()), m_Direction(Vector2f()), m_Dashes(1), m_Collider(nullptr), m_ColliderLeft(nullptr),
  m_ColliderRight(nullptr), m_Sprite(nullptr), m_Particle(nullptr), m_IsGrounded(false), m_Stamina(PLAYER_BASE_STAMINA), m_CanHold(false)
{
  std::cout << "Creating player at (" << position.x << ", " << position.y << ')' << std::endl;

  // All the player animations
  m_Sprite = new Sprite(Point2f{ PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE }, FRAMES_PER_SECOND, PLAYER_IDLE_RESOURCE);
  m_Sprite->AddResource(PLAYER_WALK_RESOURCE);
  m_Sprite->AddResource(PLAYER_CLIMB_RESOURCE);
  m_Sprite->AddResource(PLAYER_SLIDE_RESOURCE);
  m_Sprite->AddResource(PLAYER_DUCK_RESOURCE);
  m_Sprite->AddResource(PLAYER_SLIDE_RESOURCE);
  m_Sprite->AddResource(PLAYER_DUCK_RESOURCE);
  m_Sprite->AddResource(PLAYER_FALL_RESOURCE);
  m_Sprite->AddResource(PLAYER_JUMP_RESOURCE);
  m_Sprite->AddResource(PLAYER_PUSH_RESOURCE);

  m_Hair = new Hair(m_Position, 3, HAIR_SCALE);

  const Color4f colliderColor{ 0.f, 5.f, 0.f, 0.5f };
  const Color4f colliderSideColor{ 0.f, 0.f, .5f, 0.5f };

  m_Collider = new RectangleShape(Point2f{ PLAYER_BODY_WIDTH, PLAYER_BODY_HEIGHT }, m_Position, colliderColor, true);
  m_ColliderLeft = new RectangleShape(Point2f{ PIXEL_SCALE, PLAYER_BODY_HEIGHT }, Point2f{ m_Position.x - 5.f, m_Position.y }, colliderSideColor, true);
  m_ColliderRight = new RectangleShape(Point2f{ PIXEL_SCALE, PLAYER_BODY_HEIGHT }, Point2f{ m_Position.x + 5.f, m_Position.y }, colliderSideColor, true);
}

Player::~Player()
{
  std::cout << "Deleting player" << std::endl;
  delete m_Sprite;
  delete m_Particle;
  delete m_Hair;
  delete m_Collider;
  delete m_ColliderLeft;
  delete m_ColliderRight;
}

void Player::Draw(bool debug) const
{
  // Draw the hair behind the player
  const float half{ PLAYER_SCALE / 2.f };
  const bool flipped{ m_Direction.x < 0.f };

  m_Hair->Draw(flipped, debug);
  m_Sprite->Draw(m_Position, PLAYER_SCALE, flipped, debug);

  if (debug) {
    m_Collider->Draw();
    m_ColliderLeft->Draw();
    m_ColliderRight->Draw();

    // Draw the circle of the actual player position value
    SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });
    FillEllipse(m_Position, 5.f, 5.f);

    // Draw the direction vector
    SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
    DrawLine(m_Position + half, m_Position + half + Vector2f(m_Direction * 10.f).ToPoint2f());
  }
}

void Player::Update(float elapsedSec, const Tilemap& tilemap)
{
  // Reset grounded, the collision handling will check if the player is grounded
  m_IsGrounded = false;

  switch (m_State) {
    case Player::State::Idle:
    {
      m_Dashes = 1;
      m_Stamina = PLAYER_BASE_STAMINA;
      break;
    }
    case Player::State::Holding:
    {
      m_Stamina -= HOLDING_STAMINA_COST * elapsedSec;
      m_Velocity.x = 0;
      break;
    }
    case Player::State::Climbing:
    {
      m_Stamina -= CLIMBING_STAMINA_COST * elapsedSec;
      break;
    }
  }

  // Multiple iteration collision handling
  HandleCollision(elapsedSec, tilemap);

  const float half{ PLAYER_SCALE / 2.f };
  if (m_IsGrounded) {
    const Point2f newPos = MathUtils::Lerp(m_Hair->GetPosition(), Point2f{ m_Position.x + half, m_Position.y }, 0.05f);
    m_Hair->SetEnd(newPos); // Slowly move the hair toward the desired position
  }

  m_Sprite->SetState((int)m_State);

  SetPosition(Point2f{
    m_Position.x + m_Velocity.x * elapsedSec,
    m_Position.y + m_Velocity.y * elapsedSec
  });

  //m_Hair->SetVelocity(Vector2f(-m_Velocity.x, -m_Velocity.y));
  //m_Hair->ApplyForce(GRAVITY * elapsedSec);

  m_Hair->Update(elapsedSec);

  // Update the components
  //m_JumpParticle->Update(elapsedSec);
  m_Sprite->Update(elapsedSec);
  //m_Particle->Update(elapsedSec);
      
  // Update velocity
  m_Velocity = Vector2f{
    MathUtils::Lerp(m_Velocity.x, 0.f, 0.3f),
    m_Velocity.y,
  };

  m_CanHold = tilemap.IsTile(m_ColliderLeft->GetShape()) || tilemap.IsTile(m_ColliderRight->GetShape());
    
  if ((m_Velocity.y != 0) && (m_State == State::Holding || m_State == State::Climbing) && m_CanHold) {
    m_State = State::Climbing;
  } else if (m_Velocity.y > 0) {
    m_State = State::Jumping;
  } else if (m_Velocity.y < -50 && m_State != State::Sliding) {
    m_State = State::Falling;
  } else if (m_Velocity.x > 1 || m_Velocity.x < -1) {
    m_State = State::Running;
  } else if (m_Stamina <= 0) {
    m_State = State::Falling;
  } else if (m_IsGrounded) {
    m_State = State::Idle;
  }

  //m_Direction = Vector2f();
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
  if (m_IsGrounded || m_State == State::Holding || m_State == State::Sliding) {
    m_Velocity.y = 1000;
  }
}

void Player::Dash()
{
  if (m_Dashes < 1) {
    return;
  }

  m_Velocity = m_Direction * PLAYER_DASH_FORCE;
  m_State = State::Dashing;
  --m_Dashes;
}

void Player::Hold()
{
  if (m_CanHold) {
    m_Velocity.y = 0;
    m_State = Player::State::Holding;
  }
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
    return m_Collider;
}

void Player::SetPosition(const Point2f& position)
{
  m_Position = position;
  const float half{ PLAYER_SCALE / 2.f };

  const Rectf colliderSize{ m_Collider->GetBoundingBox() };

  m_Collider->SetPosition(Point2f{
    m_Position.x + half - colliderSize.width / 2.f,
    m_Position.y
  });

  // Side colliders for hold detection
  m_ColliderLeft->SetPosition(Point2f{ 
    m_Position.x + half - colliderSize.width / 2.f - 2.f,
    m_Position.y + (colliderSize.height - m_ColliderLeft->GetBoundingBox().height) / 2.f
  });

  m_ColliderRight->SetPosition(Point2f{ 
    m_Position.x + half + colliderSize.width / 2.f - m_ColliderRight->GetBoundingBox().width +2.f,
    m_Position.y + (colliderSize.height - m_ColliderRight->GetBoundingBox().height) / 2.f
  });

  m_Hair->SetGoal(Point2f{ m_Position.x + PLAYER_HAIR_START, m_Position.y + PLAYER_HEIGHT});
}

void Player::SetVelocity(const Vector2f& velocity)
{
  m_Velocity = velocity;
}

void Player::Up()
{
  m_Direction.y = 1;

  if (m_State == State::Holding || m_State == State::Climbing) {
    m_Velocity.y = 100;
  }
}

void Player::Down()
{
  m_Direction.y = -1;

  if (m_State == State::Idle) {
    m_State = State::Crouching;
  } else if (m_State == State::Holding || m_State == State::Climbing) {
    m_Velocity.y = -100;
  }
}

void Player::Left()
{
  m_Direction.x = -1;

  m_Velocity.x = m_Direction.x * PLAYER_ACCELERATION; // Too lazy to type a -
}

void Player::Right()
{
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

void Player::HandleCollision(float elapsedSec, const Tilemap& tilemap)
{
  const Point2f nextPlayerPos{
    GetCenter().x + m_Velocity.x * elapsedSec,
    m_Position.y + m_Velocity.y * elapsedSec
  };

  const Rectf collisionRect{
    m_Collider->GetShape()
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
    if (m_Velocity.y < 0 && tilemap.IsTile(Rectf{ nextCollisionRect.left + 2.f, nextCollisionRect.bottom, nextCollisionRect.width - 4.f, nextCollisionRect.height / 4.f }) || tilemap.IsTile(nextPlayerPos))
    {
      // Player collided with the ground
      m_IsGrounded = true;
      m_Velocity.y = 0.f;
    }

    // Check if the collision happens on the top side
    if (m_Velocity.y > 0 && tilemap.IsTile(Rectf{ nextCollisionRect.left + 2.f, nextCollisionRect.bottom + nextCollisionRect.height - (nextCollisionRect.height / 2.f), nextCollisionRect.width - 4.f, nextCollisionRect.height / 4.f}))
    {
      // Player collided with the ceiling
      m_Velocity.y = 0.f;
    }

    // Check if the collision happens on the horizontal sides
    if (m_Velocity.x != 0 && 
        tilemap.IsTile(Rectf{ nextCollisionRect.left, nextCollisionRect.bottom + 2.f, nextCollisionRect.width / 4.f, nextCollisionRect.height - 4.f}) || 
        tilemap.IsTile(Rectf{ nextCollisionRect.left + nextCollisionRect.width - (nextCollisionRect.width / 4.f), nextCollisionRect.bottom + 2.f, nextCollisionRect.width / 4.f, nextCollisionRect.height - 4.f})
        )
    {
      // Player collided with the wall
      m_Velocity.x = 0.f;

      if (m_IsGrounded) {
       m_State = State::Pushing;
      } else {
       m_State = State::Sliding;
      }
    }
  }
}
