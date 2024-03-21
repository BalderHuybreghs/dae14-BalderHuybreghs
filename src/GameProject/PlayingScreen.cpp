#include "pch.h"
#include "PlayingScreen.h"
#include "GameDefines.h"
#include "utils.h"
#include "RectangleShape.h"
#include <vector>
#include "EditorScreen.h"

using namespace utils;

PlayingScreen::PlayingScreen(const std::string& levelName)
  : m_LevelName(levelName), m_LevelPtr(nullptr), m_PlayerPtr(nullptr), m_CameraPtr(nullptr), m_TilemapPtr(nullptr)
{
  
}

PlayingScreen::~PlayingScreen()
{
  delete m_LevelPtr;
  delete m_CameraPtr;
  delete m_PlayerPtr;
}

void PlayingScreen::Initialize()
{
  m_LevelPtr = new Level(m_LevelName, "snow", TILEMAP_BG_PREFIX + "dirt");

  // Load the level from disk
  m_LevelPtr->Load();

  m_PlayerPtr = new Player(m_LevelPtr->GetPlayerSpawn());

  const Point2f playerPos{ m_PlayerPtr->GetPosition() };
  m_CameraPtr = new Camera(m_PlayerPtr->GetPosition());
  m_TilemapPtr = m_LevelPtr->GetFrontTilemap();

  // Build the level at the end of creation
  m_LevelPtr->Build();
}

void PlayingScreen::Draw()
{
  m_CameraPtr->PushMatrix();
  m_LevelPtr->Draw(true);
  m_PlayerPtr->Draw(true);
  m_CameraPtr->PopMatrix();
}

void PlayingScreen::Update(float elapsedSec)
{
  const Point2f oldPlayerPosition{ m_PlayerPtr->GetPosition() };

  m_LevelPtr->Update(*m_PlayerPtr, elapsedSec);
  m_PlayerPtr->Update(elapsedSec);

  Point2f playerPosition{ m_PlayerPtr->GetPosition() };
  Vector2f playerVelocity{ m_PlayerPtr->GetVelocity() };

  // Collision detection
  bool isOnFloor = false;
  bool isOnLeftWall = false;
  bool isOnRightWall = false;
  bool isOnCeiling = false;

  Rectf playerShape{ m_PlayerPtr->GetCollisionShape()->GetShape() };
  for (const std::vector<Point2f>& polygon : m_LevelPtr->GetCollisionPolygons()) {
    if (IsOverlapping(polygon, playerShape)) {
      HitInfo hi{};
      bool hit{ Raycast(polygon, playerPosition, oldPlayerPosition, hi) };

      if (hit) {
        if (hi.normal.y > 0.f) {
          isOnFloor = true;
        } else if (hi.normal.y < 0.f) {
          isOnCeiling = true;
        }
        if (hi.normal.x > 0.f) {
          isOnRightWall = true;
        } else if (hi.normal.x < 0.f) {
          isOnLeftWall = true;
        }
        m_PlayerPtr->ApplyForce(playerVelocity * hi.normal);
        m_PlayerPtr->SetPosition(oldPlayerPosition);
      }

      // Break the loop only if a collision is detected, not necessarily after the first one
      // This allows handling multiple collisions
      // break;
    }
  }

  // Apply appropriate actions based on collision flags

  if (!isOnFloor) {
    m_PlayerPtr->ApplyForce(GRAVITY); // Apply gravity if not colliding with floor
  } else {
    m_PlayerPtr->SetVelocity(Vector2f{ playerVelocity.x, 0.f });
  }

  if (isOnLeftWall && playerVelocity.x < 0) {
    m_PlayerPtr->SetVelocity(Vector2f{ 0.f, playerVelocity.y });
  }

  if (isOnRightWall && playerVelocity.x > 0) {
    m_PlayerPtr->SetVelocity(Vector2f{ 0.f, playerVelocity.y });
  }

  if (isOnCeiling && playerVelocity.y > 0) {
    m_PlayerPtr->SetVelocity(Vector2f{ playerVelocity.x, 0.f });
  }

  // Handle other collision flags here if needed

  m_CameraPtr->SetPosition(m_PlayerPtr->GetPosition());
}


void PlayingScreen::OnKeyDownEvent(const SDL_KeyboardEvent& key)
{
  const Vector2f playerVel{ m_PlayerPtr->GetVelocity() };

  switch (key.keysym.sym) {
  case SDLK_a:
    m_PlayerPtr->SetVelocity(Vector2f(-500, playerVel.y));
    break;
  case SDLK_d:
    m_PlayerPtr->SetVelocity(Vector2f(500, playerVel.y));
    break;
  case SDLK_w:

    //for (const std::vector<Point2f>& polygon : m_LevelPtr->GetCollisionPolygons()) {
    //  if (IsOverlapping(polygon, m_PlayerPtr->GetCollisionShape()->GetShape())) {
        m_PlayerPtr->ApplyForce(Vector2f(0.f, 2000));
    //  }
    //}
    break;
  case SDLK_s:
    m_PlayerPtr->Crouch();
    break;
  
  // Switch to level editor
  case SDLK_TAB:
    m_ScreenManagerPtr->SetScreen(new EditorScreen(m_LevelName, m_PlayerPtr->GetPosition()));
    break;
  }
}

void PlayingScreen::OnKeyUpEvent(const SDL_KeyboardEvent& key)
{
}

void PlayingScreen::OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
}

void PlayingScreen::OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void PlayingScreen::OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}

bool PlayingScreen::ResolveCollision(Player& player, const Point2f& oldPosition, const std::vector<Point2f>& collisionPolygon)
{
  const Point2f playerPos{ player.GetPosition() };

  HitInfo hi{};
  bool hit{ Raycast(collisionPolygon, playerPos, oldPosition, hi) };

  if (hit) {
    const Vector2f playerVel{ player.GetVelocity() };
    m_PlayerPtr->ApplyForce((playerVel * hi.normal));
  }

  m_PlayerPtr->SetPosition(hi.intersectPoint);
  return hit;
}
