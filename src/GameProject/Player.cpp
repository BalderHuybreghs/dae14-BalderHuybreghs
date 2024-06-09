#include "pch.h"
#include "Player.h"
#include <iostream>
#include "GameDefines.h"
#include "RectangleShape.h"
#include "utils.h"
#include "MathUtils.h"
#include "ResourceUtils.h"
#include "structs.h"
#include "TextureManager.h"
#include <cmath>

using namespace utils;

Player::Player(const Point2f& position, const InputManager* inputManagerPtr)
  : m_State(Player::State::Idle), m_Position(position), m_Velocity(Vector2f()), m_Direction(Vector2f()), m_Dashes(1), m_ColliderPtr(nullptr),
  m_SpritePtr(nullptr), m_ParticleSpritePtr(nullptr), m_IsGrounded(false), m_Stamina(PLAYER_BASE_STAMINA), m_CanHold(false), m_Flipped(false),
  m_InputManagerPtr(inputManagerPtr), m_IsHoldingSpace(false), m_DashCooldown(0), m_RespawnPoint(position), m_DeathSecs(0), m_ShouldRespawn(false), 
  m_SpawnProtection(0), m_HoldCooldown(0), m_Timer(0)
{
  std::cout << "Creating player at (" << position.x << ", " << position.y << ')' << std::endl;

  // All the player animations
  m_SpritePtr = new Sprite(Point2f{ PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE }, FRAMES_PER_SECOND, "idle", PLAYER_IDLE_RESOURCE);
  m_SpritePtr->AddState("run", PLAYER_RUN_RESOURCE);
  m_SpritePtr->AddState("climb", PLAYER_CLIMB_RESOURCE);
  m_SpritePtr->AddState("hold", PLAYER_SLIDE_RESOURCE);
  m_SpritePtr->AddState("duck", PLAYER_DUCK_RESOURCE);
  m_SpritePtr->AddState("fall", PLAYER_FALL_RESOURCE);
  m_SpritePtr->AddState("jump", PLAYER_JUMP_RESOURCE);
  m_SpritePtr->AddState("push", PLAYER_PUSH_RESOURCE);
  m_SpritePtr->AddState("death", PLAYER_DEATH_RESOURCE);

  m_HairPtr = new Hair(m_Position, 3, HAIR_SCALE);
  m_BallTexturePtr = TextureManager::GetInstance()->GetTexture(HAIR_RESOURCE); // The ball texture is the same as a hair texture

  const Color4f colliderColor{ 0.f, 5.f, 0.f, 0.5f };
  const Color4f colliderSideColor{ 0.f, 0.f, .5f, 0.5f };

  m_ColliderPtr = new RectangleShape(Point2f{ PLAYER_BODY_WIDTH, PLAYER_BODY_HEIGHT }, m_Position, colliderColor, true);

  m_DashSoundPtr = new SoundEffect(ResourceUtils::ResourceToSoundPath("player/dash"));
  m_DashSoundPtr->SetVolume(5);

  m_DeathSoundPtr = new SoundEffect(ResourceUtils::ResourceToSoundPath("player/death"));
  m_DeathSoundPtr->SetVolume(5);

  m_PreDeathSoundPtr = new SoundEffect(ResourceUtils::ResourceToSoundPath("player/pre_death"));
  m_PreDeathSoundPtr->SetVolume(5);
}

Player::~Player()
{
  std::cout << "Deleting player" << std::endl;
  delete m_SpritePtr;
  delete m_ParticleSpritePtr;
  delete m_HairPtr;
  delete m_ColliderPtr;
  delete m_DashSoundPtr;
  delete m_DeathSoundPtr;
  delete m_PreDeathSoundPtr;
}

void Player::Draw(bool debug) const
{
  // Draw the hair behind the player
  const float half{ PLAYER_SCALE / 2.f };
  const bool flipped{ m_Flipped };

  // I attempted to draw the player between the bangs and the hair...
  const Color4f hairColor{ m_Dashes > 0 ? PLAYER_DASH_HAIR_COLOR : PLAYER_NO_DASH_HAIR_COLOR };
  if (!m_ShouldRespawn) {
    if (m_State != State::Dead) {
      m_HairPtr->Draw(hairColor, flipped, debug);
      m_HairPtr->DrawBangs(m_Position, hairColor, flipped, debug);
    }

    Color4f playerColor{ 1.f, 1.f, 1.f, 1.f };

    if (m_Stamina <= 0.f) {
      // Source: https://en.cppreference.com/w/cpp/numeric/math/fmod
      float flashCycleTime = fmod(m_Timer, PLAYER_FLASH_TIME * 2);

      // Determine the color based on the flash cycle time
      if (flashCycleTime < PLAYER_FLASH_TIME) {
        playerColor = Color4f{ .8f, 0.f, 0.f, 1.f };
      } else {
        playerColor = Color4f{ 1.f, 1.f, 1.f, 1.f };
      }
    }

    m_SpritePtr->DrawColor(Rectf{ m_Position.x, m_Position.y, PLAYER_SCALE, PLAYER_SCALE }, playerColor, flipped, debug);

  } else {
    // Draw all the hair balls rotating around the player
    const float perc{ m_DeathSecs / DEATH_ANIM_TIME };
    const float circ{ (float)M_PI * 2.f };
    const Point2f center{ m_ColliderPtr->GetShape().Center() };

    for (int index{ 0 }; index < 8; index++)
    {
      const Rectf dstRect{
        center.x + cosf((float)index / 8 * circ - perc * circ) * ((1 - perc) * 50.f),
        center.y + sin((float)index / 8 * circ - perc * circ) * ((1 - perc) * 50.f),
        8 * PIXEL_SCALE,
        8 * PIXEL_SCALE
      };

      m_BallTexturePtr->DrawColor(dstRect, hairColor);
    }
  }


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
  if (m_State != State::Dashing || m_DashCooldown > 0.f) {
    m_Velocity = Vector2f{
      MathUtils::Lerp(m_Velocity.x, 0.f, 0.3f),
      m_Velocity.y,
    };
  }

  // Clamp the velocity between the terminal velocity
  m_Velocity = Vector2f{
    MathUtils::Clamp(m_Velocity.x, -TERMINAL_VELOCITY.x, TERMINAL_VELOCITY.x),
    MathUtils::Clamp(m_Velocity.y, -TERMINAL_VELOCITY.y, TERMINAL_VELOCITY.y)
  };

  HandleCollision(elapsedSec, tilemap);

  const float half{ PLAYER_SCALE / 2.f };
  if (floorf(m_Velocity.y) == 0.f) {
    const Point2f newPos = MathUtils::Lerp(m_HairPtr->GetPosition(), Point2f{ m_Position.x + half, m_Position.y }, 0.1f);
    m_HairPtr->SetEnd(newPos); // Slowly move the hair toward the desired position
  }

  if (!m_ShouldRespawn) {
    SetPosition(Point2f{
      m_Position.x + m_Velocity.x * elapsedSec,
      m_Position.y + m_Velocity.y * elapsedSec
    });
  }

  m_IsHoldingSpace = m_InputManagerPtr->IsKeyDown(SDLK_SPACE);

  switch (m_State) {
    case Player::State::Idle:
    {
      if (m_InputManagerPtr->IsKeyDown(SDLK_j)) {
        if (((tilemap.IsTile(GetLeftHoldRect(collider)) && m_Flipped) || (tilemap.IsTile(GetRightHoldRect(collider)) && !m_Flipped)) && m_HoldCooldown <= 0.f) {
          m_State = State::Climbing;
          m_Velocity.y = 0.f;
          break;
        }
      }

      if (m_InputManagerPtr->IsKeyDown(SDLK_SPACE) && m_IsGrounded) {
        m_Position.y += 10.f;
        m_Velocity.y = PLAYER_JUMP_FORCE;
      }

      if (m_InputManagerPtr->IsKeyDown(SDLK_a) || m_InputManagerPtr->IsKeyDown(SDLK_d)) {
        if (m_InputManagerPtr->IsKeyDown(SDLK_SPACE) && m_IsGrounded) {
          m_Position.y += 10.f;
          m_Velocity.y = PLAYER_JUMP_FORCE;
        }

        if (floorf(std::abs(m_Velocity.x)) <= 0.1f && tilemap.IsTile(GetGroundedRect(m_ColliderPtr->GetShape()))) {
          m_SpritePtr->SetState("push");
          m_HairPtr->SetState("push");
        } else if ((tilemap.IsTile(GetLeftHoldRect(collider)) || tilemap.IsTile(GetRightHoldRect(collider))) && m_HoldCooldown <= 0.f) {
          m_State = State::Sliding;
        } else {
          m_SpritePtr->SetState("run");
          m_HairPtr->SetState("run");
        }
      } else if (m_InputManagerPtr->IsKeyDown(SDLK_s) && m_IsGrounded) {
        m_State = State::Crouching;
      } else if (!m_IsGrounded) {
        if (m_Velocity.y > 0.f) {
          m_SpritePtr->SetState("jump");
          m_HairPtr->SetState("jump");
        } else {
          m_SpritePtr->SetState("fall");
          m_HairPtr->SetState("fall");
        }
      } else {
        m_SpritePtr->SetState("idle");
        m_HairPtr->SetState("idle");
      }

      if (m_IsGrounded) {
        m_Dashes = 1;
        m_Stamina = PLAYER_BASE_STAMINA;
      }
      break;
    }
    case Player::State::Sliding:
    {
      if (!((m_InputManagerPtr->IsKeyDown(SDLK_a) && m_Flipped) || (m_InputManagerPtr->IsKeyDown(SDLK_d) && !m_Flipped)) || m_IsGrounded) {
        m_State = State::Idle;
        break;
      }

      if (!(tilemap.IsTile(GetLeftHoldRect(collider)) || tilemap.IsTile(GetRightHoldRect(collider)))) {
        m_State = State::Idle;
        break;
      }

      if (tilemap.IsTile(GetGroundedRect(m_ColliderPtr->GetShape()))) {
        m_State = State::Idle;
        break;
      }

      if (m_InputManagerPtr->IsKeyDown(SDLK_SPACE)) {
        float angle{ ((m_Flipped ? 135 : 45) / 360.f) * (float)M_PI * 2.f };
        m_Velocity.x = PLAYER_JUMP_FORCE * cosf(angle);
        m_Velocity.y = PLAYER_JUMP_FORCE * sinf(angle);
        m_State = State::Idle;
        break;
      }

      if (m_InputManagerPtr->IsKeyDown(SDLK_j) && m_Stamina >= 0.f) {
        m_Velocity.y = 0.f;
        m_State = State::Climbing;
      }

      m_SpritePtr->SetState("hold");
      m_HairPtr->SetState("hold");
      break;
    }
    case Player::State::Climbing:
    {
      if (!m_InputManagerPtr->IsKeyDown(SDLK_j) || m_Stamina <= 0.f) {
        m_State = State::Sliding;
        break;
      }

      if (!(tilemap.IsTile(GetLeftHoldRect(collider)) || tilemap.IsTile(GetRightHoldRect(collider)))) {
        m_State = State::Idle;
        break;
      }

      if (m_IsGrounded) {
        m_Velocity.y = std::max(0.f, m_Velocity.y);
        break;
      }

      if (!(m_InputManagerPtr->IsKeyDown(SDLK_s) || m_InputManagerPtr->IsKeyDown(SDLK_w))) {
        m_SpritePtr->SetState("hold");
        m_HairPtr->SetState("hold");
        m_Position.y += 33.f * elapsedSec; // Correct for the strange sliding bug
      } else {
        m_SpritePtr->SetState("climb");
        m_HairPtr->SetState("climb");
      }

      if (m_InputManagerPtr->IsKeyDown(SDLK_SPACE)) {
        float angle{ ((m_Flipped ? 135 : 45) / 360.f) * (float)M_PI * 2.f };
        m_Velocity.x = PLAYER_JUMP_FORCE * cosf(angle);
        m_Velocity.y = PLAYER_JUMP_FORCE * sinf(angle);
        m_State = State::Idle;
        m_HoldCooldown = .2f;
        break;
      }

      m_Velocity.y = 0.f;
      m_Velocity.x = 0.f;
      m_Stamina -= CLIMBING_STAMINA_COST * elapsedSec;
      break;
    }
    case Player::State::Dashing:
    {
      m_SpritePtr->SetState("duck");
      m_HairPtr->SetState("duck");
      if (m_SpritePtr->IsAnimationDone() || m_IsGrounded) {
        m_State = State::Idle;
        break;
      }
      break;
    }

    case Player::State::Crouching:
    {
      if (!m_InputManagerPtr->IsKeyDown(SDLK_s)) {
        m_State = State::Idle;
      }

      m_SpritePtr->SetState("duck");
      m_HairPtr->SetState("duck");
      m_Velocity.x = 0.f;

      break;
    }

    case Player::State::Dead:
    {
      if (!m_ShouldRespawn) {
        m_SpritePtr->SetState("death");
        m_Velocity = {};

        if (m_SpritePtr->IsAnimationDone()) {
          // Play the other death animation
          m_DeathSecs = DEATH_ANIM_TIME;
          m_ShouldRespawn = true;
          m_DeathSoundPtr->Play(0);
        }
      }

      // Respawn the player if the death animation is done
      if (m_DeathSecs > 0.f) {
        m_DeathSecs -= elapsedSec;
      } else if (m_ShouldRespawn) {
        m_Position = m_RespawnPoint;
        m_Velocity = Vector2f{};
        m_State = State::Idle;
        m_DeathSecs = 0.f;
        m_ShouldRespawn = false;
        m_Dashes = 1;
        m_DashCooldown = 0.f;
        m_IsGrounded = true;
        m_SpawnProtection = 0.2f;
      }

      break;
    }
  }

  m_HairPtr->Update(elapsedSec);

  // Play the animation faster when the player is dying
  m_SpritePtr->Update(m_State == State::Dead ? elapsedSec * 3 : elapsedSec);

  m_IsGrounded = tilemap.IsTile(GetGroundedRect(collider));

  if (m_InputManagerPtr->IsKeyDown(SDLK_LSHIFT) && m_Dashes > 0 && m_State != State::Dashing && m_State != State::Dead && m_DashCooldown <= 0.f) {
    m_DashSoundPtr->Play(0);
    m_State = State::Dashing;
    m_Velocity = m_Direction * PLAYER_DASH_FORCE;
    --m_Dashes;
    m_DashCooldown = 1.0f;
  }

  m_Flipped = (m_Flipped && m_Direction.x == 0.f) || m_Direction.x < 0.f;
  m_Direction = Vector2f();

  m_DashCooldown = std::max(0.f, m_DashCooldown - elapsedSec);
  m_SpawnProtection = std::max(0.f, m_SpawnProtection - elapsedSec);
  m_HoldCooldown = std::max(0.f, m_HoldCooldown - elapsedSec);

  m_Timer += elapsedSec;
}

void Player::RefillDashes(int amount)
{
  if (amount < 0) {
    std::cout << "Attempt to fill dashes with negative value" << std::endl;
    return;
  }

  m_Dashes += amount;
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
  if (m_State == State::Dead) {
    return;
  }

  m_Direction.y = 1;

  if (m_State == State::Climbing) {
    m_Velocity.y = 100;
  }
}

void Player::Down()
{
  if (m_State == State::Climbing) {
    m_Velocity.y = -100;
  }
}

void Player::Left()
{
  if (m_State == State::Climbing || m_State == State::Crouching || m_State == State::Dead) {
    return;
  }

  m_Direction.x = -1;

  m_Velocity.x = m_Direction.x * PLAYER_ACCELERATION; // Too lazy to type a -
}

void Player::Right()
{
  if (m_State == State::Climbing || m_State == State::Crouching || m_State == State::Dead) {
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

bool Player::IsHoldingSpace() const
{
  return m_IsHoldingSpace;
}

void Player::Kill()
{
  if (m_State == State::Dead || m_SpawnProtection > 0.f) {
    return;
  }

  m_PreDeathSoundPtr->Play(0);
  m_State = State::Dead; // No matter the state, when the player is killed, they die... Who would have thought?
  m_Velocity = Vector2f{}; // Instantly kill any velocity going on
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
    if (tilemap.IsTile(GetBottomCollisionRect(nextCollisionRect)) || tilemap.IsTile(nextPlayerPos))
    {
      // Player collided with the ground
      m_Velocity.y = 0.f;
    }

    // Check if the collision happens on the top side
    if (tilemap.IsTile(GetTopCollisionRect(nextCollisionRect)))
    {
      // Player collided with the ceiling
      m_Velocity.y = 0.f;
      m_Position.y -= 1.5f;
    }

    // Check if the collision happens on the horizontal sides
    if (tilemap.IsTile(GetLeftCollisionRect(nextCollisionRect)) || 
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

int Player::GetDashes() const
{
  return m_Dashes;
}

float Player::GetStamina() const
{
  return m_Stamina;
}

void Player::SetDashCooldown(float secs)
{
  m_DashCooldown = secs;
}

Point2f Player::GetRespawnPoint() const
{
  return m_RespawnPoint;
}

void Player::SetRespawnPoint(const Point2f& position)
{
  m_RespawnPoint = position;
}

void Player::SetArtificalGrounded()
{
  m_IsGrounded = true;
}
