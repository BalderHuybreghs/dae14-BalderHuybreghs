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
  : m_State(Player::State::Idle), m_Position(position), m_Velocity(Vector2f()), m_Direction(Vector2f()), m_Dashes(1), m_ColliderPtr(nullptr), m_ColliderLeftPtr(nullptr),
  m_ColliderRightPtr(nullptr), m_SpritePtr(nullptr), m_ParticleSpritePtr(nullptr), m_IsGrounded(false), m_Stamina(PLAYER_BASE_STAMINA), m_CanHold(false), m_Flipped(false),
  m_InputManagerPtr(inputManagerPtr)
{
  std::cout << "Creating player at (" << position.x << ", " << position.y << ')' << std::endl;

  // All the player animations
  m_SpritePtr = new Sprite(Point2f{ PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE }, FRAMES_PER_SECOND, "idle", PLAYER_IDLE_RESOURCE);
  m_SpritePtr->AddState("walk", PLAYER_WALK_RESOURCE);
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
  m_ColliderLeftPtr = new RectangleShape(Point2f{ PIXEL_SCALE, PLAYER_BODY_HEIGHT }, Point2f{ m_Position.x - 5.f, m_Position.y }, colliderSideColor, true);
  m_ColliderRightPtr = new RectangleShape(Point2f{ PIXEL_SCALE, PLAYER_BODY_HEIGHT }, Point2f{ m_Position.x + 5.f, m_Position.y }, colliderSideColor, true);
}

Player::~Player()
{
  std::cout << "Deleting player" << std::endl;
  delete m_SpritePtr;
  delete m_ParticleSpritePtr;
  delete m_HairPtr;
  delete m_ColliderPtr;
  delete m_ColliderLeftPtr;
  delete m_ColliderRightPtr;
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
    m_ColliderLeftPtr->Draw();
    m_ColliderRightPtr->Draw();

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
  m_Direction = m_Direction.Normalized();

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
    case Player::State::Dashing:
    {
      m_Velocity = m_Direction * PLAYER_DASH_FORCE;
    }
  }

  // Multiple iteration collision handling
  HandleCollision(elapsedSec, tilemap);

  const float half{ PLAYER_SCALE / 2.f };
  if (m_IsGrounded) {
    const Point2f newPos = MathUtils::Lerp(m_HairPtr->GetPosition(), Point2f{ m_Position.x + half, m_Position.y }, 0.05f);
    m_HairPtr->SetEnd(newPos); // Slowly move the hair toward the desired position
  }

  SetPosition(Point2f{
    m_Position.x + m_Velocity.x * elapsedSec,
    m_Position.y + m_Velocity.y * elapsedSec
  });

  //m_Hair->SetVelocity(Vector2f(-m_Velocity.x, -m_Velocity.y));
  //m_Hair->ApplyForce(GRAVITY * elapsedSec);

  m_HairPtr->Update(elapsedSec);

  // Update the components
  //m_JumpParticle->Update(elapsedSec);
  m_SpritePtr->Update(elapsedSec);
  //m_Particle->Update(elapsedSec);
      
  // Update velocity
  m_Velocity = Vector2f{
    MathUtils::Lerp(m_Velocity.x, 0.f, 0.3f),
    m_Velocity.y,
  };

  m_CanHold = tilemap.IsTile(m_ColliderLeftPtr->GetShape()) || tilemap.IsTile(m_ColliderRightPtr->GetShape());
    
  if (m_CanHold && !m_IsGrounded && ((m_InputManagerPtr->IsKeyDown(SDLK_LEFT) && m_Flipped) || (m_InputManagerPtr->IsKeyDown(SDLK_RIGHT) && !m_Flipped))) {
    m_State = State::Sliding;
  } else if ((m_Velocity.y != 0) && (m_State == State::Holding || m_State == State::Climbing) && m_CanHold) {
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

  m_Flipped = (m_Flipped && m_Direction.x == 0.f) || m_Direction.x < 0.f;
  m_Direction = Vector2f();
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
    m_Velocity.y = PLAYER_JUMP_FORCE;
  }
}

void Player::Dash()
{
  if (m_Dashes < 1) {
    return;
  }

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

  // Side colliders for hold detection
  m_ColliderLeftPtr->SetPosition(Point2f{ 
    m_Position.x + half - colliderSize.width / 2.f - 2.f,
    m_Position.y + (colliderSize.height - m_ColliderLeftPtr->GetBoundingBox().height) / 2.f
  });

  m_ColliderRightPtr->SetPosition(Point2f{ 
    m_Position.x + half + colliderSize.width / 2.f - m_ColliderRightPtr->GetBoundingBox().width +2.f,
    m_Position.y + (colliderSize.height - m_ColliderRightPtr->GetBoundingBox().height) / 2.f
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
  if (m_State == State::Climbing || m_State == State::Holding || m_State == State::Crouching) {
    return;
  }

  m_Direction.x = -1;

  m_Velocity.x = m_Direction.x * PLAYER_ACCELERATION; // Too lazy to type a -
}

void Player::Right()
{
  if (m_State == State::Climbing || m_State == State::Holding || m_State == State::Crouching) {
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
    if (m_Velocity.y < 0 && tilemap.IsTile(Rectf{ nextCollisionRect.left + 2.f, nextCollisionRect.bottom, nextCollisionRect.width - 4.f, nextCollisionRect.height / 4.f }) || tilemap.IsTile(nextPlayerPos))
    {
      // Player collided with the ground
      m_IsGrounded = true;
      m_Velocity.y = 0.f;
    }

    // Check if the collision happens on the top side
    if (m_Velocity.y > 0 && tilemap.IsTile(Rectf{ nextCollisionRect.left + 2.f, nextCollisionRect.bottom + nextCollisionRect.height, nextCollisionRect.width - 4.f, nextCollisionRect.height / 4.f}))
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
