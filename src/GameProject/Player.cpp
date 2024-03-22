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
  : m_State(Player::State::Idle), m_Position(position), m_Velocity(Vector2f()), m_Dashes(1), m_Collider(nullptr), m_ColliderSlideLeft(nullptr),
  m_ColliderSlideRight(nullptr), m_ColliderFeet(nullptr), m_Sprite(nullptr), m_Particle(nullptr), m_IsGrounded(false)
{
  std::cout << "Creating player at (" << position.x << ", " << position.y << ')' << std::endl;

  // All the player animations
  m_Sprite = new Sprite(m_Position, Point2f{ 200.f, 200.f}, Point2f{ FRAME_SIZE, FRAME_SIZE }, FRAMES_PER_SECOND, PLAYER_IDLE_RESOURCE);
  m_Sprite->AddResource(PLAYER_WALK_RESOURCE);
  m_Sprite->AddResource(PLAYER_CLIMB_RESOURCE);
  m_Sprite->AddResource(PLAYER_EDGE_RESOURCE);
  m_Sprite->AddResource(PLAYER_DUCK_RESOURCE);
  m_Sprite->AddResource(PLAYER_EDGE_RESOURCE);
  m_Sprite->AddResource(PLAYER_DUCK_RESOURCE);
  m_Sprite->AddResource(PLAYER_FALL_RESOURCE);
  m_Sprite->AddResource(PLAYER_JUMP_RESOURCE);

  const Color4f debugColor{ 0.f, 5.f, 0.f, 0.5f };
  m_Collider = new RectangleShape(Point2f{ 50.f, 70.f }, m_Position, debugColor, true);
  m_ColliderSlideLeft = new RectangleShape(Point2f{ 3.f, 10.f }, Point2f{ m_Position.x - 5.f, m_Position.y }, debugColor, true);
  m_ColliderSlideRight = new RectangleShape(Point2f{ 3.f, 10.f }, Point2f{ m_Position.x + 5.f, m_Position.y }, debugColor, true);
  m_ColliderFeet = new RectangleShape(Point2f{ 10.f, 3.f }, Point2f{ m_Position.x, m_Position.y + 5.f }, debugColor, true);
}

Player::~Player()
{
  std::cout << "Deleting player" << std::endl;
  delete m_Sprite;
  delete m_Particle;
  delete m_Collider;
  delete m_ColliderFeet;
  delete m_ColliderSlideLeft;
  delete m_ColliderSlideRight;
}

void Player::Draw(bool debug) const
{
  m_Sprite->Draw(debug);

  if (debug) {
    m_Collider->Draw();
    m_ColliderFeet->Draw();
    m_ColliderSlideLeft->Draw();
    m_ColliderSlideRight->Draw();

    // Draw the circle of the actual player position value
    SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });
    FillEllipse(m_Position, 5.f, 5.f);
  }
}

void Player::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& collisionPolygons)
{
  // Test for player collision in 2 iterations
  HandleCollision(elapsedSec, collisionPolygons, true);
  HandleCollision(elapsedSec, collisionPolygons);

  if (m_Velocity.y > 0) {
    m_State = State::Jumping;
  } if (m_Velocity.y < -10) {
    m_State = State::Falling;
  } else if (m_Velocity.y > 10) {
    m_State = State::Idle;
  } else if (m_Velocity.x > 1 || m_Velocity.x < -1) {
    m_State = State::Running;
  } else {
    m_State = State::Idle;
  }

  m_Sprite->SetMirror(m_Velocity.x < 0.f);
  m_Sprite->SetState((int)m_State);

  switch (m_State) {
    case Player::State::Idle: {
      m_Dashes = 0;
      break;
    }
  }

  SetPosition(Point2f{
    m_Position.x + m_Velocity.x * elapsedSec,
    m_Position.y + m_Velocity.y * elapsedSec
  });

  // Update the components
  //m_JumpParticle->Update(elapsedSec);
  m_Sprite->Update(elapsedSec);
  //m_Particle->Update(elapsedSec);

  // Update velocity
  m_Velocity = Vector2f{
    MathUtils::Lerp(m_Velocity.x, 0.f, 0.3),
    m_Velocity.y,
  };
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
  if (m_IsGrounded) {
    m_Velocity.y = 500;
  }
}

void Player::Dash(const Vector2f& direction)
{
  if (m_Dashes < 1) {
    return;
  }


}

void Player::Crouch()
{
  // TODO: update the player hitbox
  if (m_State == Player::State::Idle) {
    m_State = Player::State::Crouching;
    m_Sprite->SetState(int(m_State));
  }

  if (m_State == Player::State::Crouching) {
    m_State = Player::State::Idle;
    m_Sprite->SetState(int(m_State));
  }
}

void Player::Hold()
{
  if (m_State == Player::State::Sliding) {
    m_State = Player::State::Holding;
    m_Sprite->SetState(int(m_State));
  }
}

void Player::Move(const Vector2f& direction)
{
  m_Position.x += direction.x;
  m_Position.y += direction.y;
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
  
  const Point2f spriteSize{ m_Sprite->GetSize() };
  const Point2f colliderSide{ m_Collider->GetSimpleSize() };

  m_Sprite->SetPosition(Point2f{
      m_Position.x - spriteSize.x / 2.f,
      m_Position.y
                        });

  m_Collider->SetPosition(Point2f{
    m_Position.x - colliderSide.x / 2.f,
    m_Position.y
                          });


  m_ColliderSlideLeft->SetPosition(Point2f{ m_Position.x - 5.f, m_Position.y });
  m_ColliderSlideRight->SetPosition(Point2f{ m_Position.x + 5.f, m_Position.y });
  m_ColliderFeet->SetPosition(Point2f{ m_Position.x, m_Position.y + 5.f });
}

void Player::SetVelocity(const Vector2f& velocity)
{
  m_Velocity = velocity;
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

void Player::HandleCollision(float elapsedSec, const std::vector<std::vector<Point2f>>& polygons, bool testGrounded)
{
  // TODO: handle collision logic here
  const Point2f nextPlayerPos{
    m_Position.x + m_Velocity.x * elapsedSec,
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

  // The points of the collision rectangle
  const Point2f bottomLeft{ collisionRect.left, collisionRect.bottom };
  const Point2f topLeft{ collisionRect.left, collisionRect.bottom + collisionRect.height };
  const Point2f topRight{ collisionRect.left + collisionRect.width, collisionRect.bottom + collisionRect.height };
  const Point2f bottomRight{ collisionRect.left + collisionRect.width, collisionRect.bottom };

  const Point2f nextBottomLeft{ nextCollisionRect.left, nextCollisionRect.bottom };
  const Point2f nextTopLeft{ nextCollisionRect.left, nextCollisionRect.bottom + nextCollisionRect.height };
  const Point2f nextTopRight{ nextCollisionRect.left + nextCollisionRect.width, nextCollisionRect.bottom + nextCollisionRect.height };
  const Point2f nextBottomRight{ nextCollisionRect.left + nextCollisionRect.width, nextCollisionRect.bottom };

  HitInfo infoBottomLeft{};
  HitInfo infoBottomRight{};
  HitInfo infoTopLeft{};
  HitInfo infoTopRight{};

  // Grounded is false unless we found a collision
  if (testGrounded) {
    m_IsGrounded = false;
  }

  // Check collision for each polygon
  for (const std::vector<Point2f> polygon : polygons) {
    const bool hitBottomLeft{ Raycast(polygon, bottomLeft, nextBottomLeft, infoBottomLeft) };
    const bool hitBottomRight{ Raycast(polygon, bottomRight, nextBottomRight, infoBottomRight) };
    const bool hitTopLeft{ Raycast(polygon, topLeft, nextTopLeft, infoTopLeft) };
    const bool hitTopRight{ Raycast(polygon, topRight, nextTopRight, infoTopRight) };

    if (testGrounded) {
      m_IsGrounded = infoBottomLeft.normal.y > 0.f || infoBottomRight.normal.y > 0.f;
    }

    if (infoBottomLeft.normal.y != 0.f || infoBottomRight.normal.y != 0.f || infoTopLeft.normal.y != 0.f || infoTopRight.normal.y != 0.f) {
      m_Velocity.y = 0.f;
    }

    if (infoBottomLeft.normal.x != 0.f || infoBottomRight.normal.x != 0.f || infoTopLeft.normal.x != 0.f || infoTopRight.normal.x != 0.f) {
      m_Velocity.x = 0.f;
    }
  }
}