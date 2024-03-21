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

  Point2f newPlayerPosition{ m_PlayerPtr->GetPosition() };

  // Collision detection
  bool collided{};
  for (const std::vector<Point2f>& polygon : m_LevelPtr->GetCollisionPolygons()) {
    collided = IsOverlapping(polygon, m_PlayerPtr->GetCollisionShape()->GetShape());
    if (collided) {
      ResolveCollision(*m_PlayerPtr, oldPlayerPosition, newPlayerPosition, polygon);
      break;
    }
  }

  // Apply gravity if not colliding
  if (!collided) {
    m_PlayerPtr->ApplyForce(GRAVITY);
  } else {
    // Stop player's vertical velocity if colliding with a surface
    m_PlayerPtr->SetPosition(newPlayerPosition);
    //m_PlayerPtr->SetVelocity(Vector2f(m_PlayerPtr->GetVelocity().x, 0.0f));
  }

  //m_PlayerPtr->SetPosition(newPlayerPosition);

  m_CameraPtr->SetPosition(m_PlayerPtr->GetPosition());
}

void PlayingScreen::OnKeyDownEvent(const SDL_KeyboardEvent& key)
{
  switch (key.keysym.sym) {
  case SDLK_a:
    m_PlayerPtr->ApplyForce(Vector2f(-200, 0));
    break;
  case SDLK_d:
    m_PlayerPtr->ApplyForce(Vector2f(200, 0));
    break;
  case SDLK_w:
    m_PlayerPtr->ApplyForce(Vector2f(0, 500));
    break;
  case SDLK_s:
    m_PlayerPtr->ApplyForce(Vector2f(0, -200));
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

bool PlayingScreen::ResolveCollision(Player& player, const Point2f& oldPosition, Point2f& newPosition, const std::vector<Point2f>& collisionPolygon)
{
  Vector2f direction{ Vector2f(newPosition.x - oldPosition.x, newPosition.y - oldPosition.y) };
  float distance = direction.Length();

  HitInfo hi{};
  bool hit{ Raycast(collisionPolygon, oldPosition, newPosition, hi) };

  if (hit) {
    // Calculate the amount to move the player out of the collision
    Vector2f correction = direction.Normalized() * (distance - hi.lambda);
    newPosition = oldPosition + correction;

    // Reflect player's velocity based on collision normal
    Vector2f reflectedVelocity = player.GetVelocity().Reflect(hi.normal);

    // Apply velocity dampening factor (e.g., 0.8 for 80% dampening)
    const float velocityDamping = 0.05f; // Adjust as needed
    player.SetVelocity(reflectedVelocity * velocityDamping);
  }

  return hit;
}
