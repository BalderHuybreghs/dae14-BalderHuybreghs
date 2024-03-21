#include "pch.h"
#include "Player.h"
#include <iostream>
#include "GameDefines.h"
#include "RectangleShape.h"
#include "utils.h"

Player::Player(const Point2f& position)
  : m_State(Player::State::Idle), m_Position(position), m_Velocity(Vector2f()), m_Dashes(1), m_Collider(nullptr), m_ColliderSlideLeft(nullptr),
  m_ColliderSlideRight(nullptr), m_ColliderFeet(nullptr), m_Sprite(nullptr), m_Particle(nullptr)
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
    utils::SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });
    utils::FillEllipse(m_Position, 5.f, 5.f);
  }
}

void Player::Update(float elapsedSec)
{
  if (m_Velocity.y < -10) {
    m_State = State::Falling;
  } else if (m_Velocity.y > 10) {
    m_State = State::Idle;
  } else if (m_Velocity.x > 10 || m_Velocity.x < -10) {
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
