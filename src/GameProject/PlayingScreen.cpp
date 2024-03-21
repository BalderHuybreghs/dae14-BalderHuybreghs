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

  // Some debug tile collision drawing
  const Rectf tileRect{ m_TilemapPtr->GetTileRect(m_PlayerPtr->GetPosition()) };

  utils::SetColor(Color4f{ 0.f, 0.f, 1.f, 0.5f });

  // Collides
  bool collides{};
  for (const std::vector<Point2f> polygons : m_LevelPtr->GetCollisionPolygons()) {
    collides = IsOverlapping(polygons, m_PlayerPtr->GetCollisionShape()->GetShape());

    if (collides) {
      break;
    }
  }

  if (collides) {
    utils::FillRect(tileRect);
  } else {
    utils::DrawRect(tileRect, 2.f);
  }

  m_CameraPtr->PopMatrix();
}

void PlayingScreen::Update(float elapsedSec)
{
  const Point2f oldPlayerPosition{ m_PlayerPtr->GetPosition() };

  m_LevelPtr->Update(*m_PlayerPtr, elapsedSec);
  m_PlayerPtr->Update(elapsedSec);

  //const Rectf playerCollisionShape = m_PlayerPtr->GetCollisionShape()->GetShape();
  //const std::vector<Tilemap::TileSide> touchingSides = m_TilemapPtr->GetTouchingSides(playerCollisionShape);

  //bool isOnGround = false;

  //// Check which sides of the player's collision shape are touching tiles
  //for (const Tilemap::TileSide& side : touchingSides) {
  //  const Point2f playerPosition{ m_PlayerPtr->GetPosition() };
  //  const Vector2f playerVelocity{ m_PlayerPtr->GetVelocity() };

  //  switch (side) {
  //  case Tilemap::TileSide::Right:
  //  case Tilemap::TileSide::Left:
  //    m_PlayerPtr->SetPosition(Point2f{ oldPlayerPosition.x, playerPosition.y });
  //    m_PlayerPtr->SetVelocity(Vector2f{ 0.f, playerVelocity.y });
  //    break;
  //  case Tilemap::TileSide::Top:
  //    m_PlayerPtr->SetPosition(Point2f(playerPosition.x, oldPlayerPosition.y));
  //    m_PlayerPtr->SetVelocity(Vector2f{ playerVelocity.x, 0.f });
  //    break;
  //  case Tilemap::TileSide::Bottom:
  //    m_PlayerPtr->SetPosition(Point2f(playerPosition.x, oldPlayerPosition.y));
  //    m_PlayerPtr->SetVelocity(Vector2f{ playerVelocity.x, 0.f });
  //    isOnGround = true;
  //    break;
  //  }
  //}

  //// Apply gravity only if the player is not on the ground
  //if (!isOnGround) {
  //  m_PlayerPtr->ApplyForce(GRAVITY);
  //}

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
