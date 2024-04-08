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
  m_LevelPtr = new Level(m_LevelName);

  // Load the level from disk
  m_LevelPtr->Load();

  m_PlayerPtr = new Player(m_LevelPtr->GetPlayerSpawn());

  const Point2f playerPos{ m_PlayerPtr->GetCenter() };
  m_CameraPtr = new Camera(START_CAMERA_POSITION);
  m_TilemapPtr = m_LevelPtr->GetFrontTilemap();

  // Build the level at the end of creation
  m_LevelPtr->Build();
}

void PlayingScreen::Draw()
{
  m_CameraPtr->PushMatrix();
  m_LevelPtr->DrawBackground(*m_CameraPtr, true);
  m_PlayerPtr->Draw(true);
  m_LevelPtr->DrawForeground(*m_CameraPtr, true);
  m_CameraPtr->PopMatrix();
}

void PlayingScreen::Update(float elapsedSec)
{
  // Player controls
  const Vector2f playerVel{ m_PlayerPtr->GetVelocity() };

  if (m_InputManagerPtr->IsKeyDown(SDLK_LSHIFT)) {
    m_PlayerPtr->Dash();
  }

  if (m_InputManagerPtr->IsKeyDown(SDLK_a)) {
    m_PlayerPtr->Left();
  }

  if (m_InputManagerPtr->IsKeyDown(SDLK_d)) {
    m_PlayerPtr->Right();
  }

  if (m_InputManagerPtr->IsKeyDown(SDLK_SPACE)) {
    m_PlayerPtr->Jump();
  }

  if (m_InputManagerPtr->IsKeyDown(SDLK_j)) {
    m_PlayerPtr->Hold();
  }

  if (m_InputManagerPtr->IsKeyDown(SDLK_w)) {
    m_PlayerPtr->Up();
  }

  if (m_InputManagerPtr->IsKeyDown(SDLK_s)) {
    m_PlayerPtr->Down();
  }

  m_LevelPtr->Update(*m_PlayerPtr, *m_CameraPtr, elapsedSec);

  // Update the player with some collision polygons to collide with
  m_PlayerPtr->Update(elapsedSec, *m_TilemapPtr);

  const Point2f playerPos{ m_PlayerPtr->GetCenter() };
  const Point2f center{
    playerPos.x - WINDOW_WIDTH / 2.f,
    playerPos.y - WINDOW_HEIGHT / 2.f
  };

  // m_CameraPtr->SetPosition(center);
}


void PlayingScreen::OnKeyDownEvent(const SDL_KeyboardEvent& key)
{

  switch (key.keysym.sym) {
  // Switch to level editor
  case SDLK_TAB:
    const Point2f playerCenter{ m_PlayerPtr->GetCenter() };
    const Point2f cameraCenter{ playerCenter.x - WINDOW_WIDTH / 2.f, playerCenter.y - WINDOW_HEIGHT / 2.f };
    m_ScreenManagerPtr->SetScreen(new EditorScreen(m_LevelName, cameraCenter));
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