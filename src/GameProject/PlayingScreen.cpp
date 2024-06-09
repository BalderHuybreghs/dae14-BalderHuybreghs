#include "pch.h"
#include "PlayingScreen.h"
#include "GameDefines.h"
#include "utils.h"
#include "RectangleShape.h"
#include <vector>
#include "EditorScreen.h"

using namespace utils;

PlayingScreen::PlayingScreen(const std::string& levelName)
  : m_LevelName(levelName), m_LevelPtr(nullptr), m_PlayerPtr(nullptr), m_CameraPtr(nullptr), m_TilemapPtr(nullptr), m_Debug(false)
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

  m_PlayerPtr = new Player(m_LevelPtr->GetPlayerSpawn(), m_InputManagerPtr);

  // Calculate the right starting positions
  const Point2f playerPos{ m_PlayerPtr->GetCenter() };
  const Rectf cameraRect{ m_LevelPtr->GetCameraRect(*m_PlayerPtr) };
  const Point2f cameraPos{
    cameraRect.left,
    cameraRect.bottom
  };

  m_CameraPtr = new Camera(cameraPos);
  m_TilemapPtr = m_LevelPtr->GetFrontTilemap();

  // Build the level at the end of creation
  m_LevelPtr->Build();
}

void PlayingScreen::Draw()
{
  m_CameraPtr->PushMatrix();
  m_LevelPtr->DrawBackground(*m_CameraPtr, m_Debug);
  m_PlayerPtr->Draw(m_Debug);
  m_LevelPtr->DrawForeground(*m_CameraPtr, m_Debug);
  m_CameraPtr->PopMatrix();
}

void PlayingScreen::Update(float elapsedSec)
{
  // Player controls
  if (m_InputManagerPtr->IsKeyDown(SDLK_a)) {
    m_PlayerPtr->Left();
  }

  if (m_InputManagerPtr->IsKeyDown(SDLK_d)) {
    m_PlayerPtr->Right();
  }

  if (m_InputManagerPtr->IsKeyDown(SDLK_w)) {
    m_PlayerPtr->Up();
  }

  m_LevelPtr->Update(*m_PlayerPtr, *m_CameraPtr, elapsedSec);

  // Update the player with some collision polygons to collide with
  m_PlayerPtr->Update(elapsedSec, *m_TilemapPtr);

  m_CameraPtr->Update(elapsedSec); // Update the camera here for animating its movement
}


void PlayingScreen::OnKeyDownEvent(const SDL_KeyboardEvent& key)
{

  switch (key.keysym.sym) {
  case SDLK_F1:
    // Toggle debug
    m_Debug = !m_Debug;
    break;

  case SDLK_TAB:
    // Switch to level editor
    m_ScreenManagerPtr->SetScreen(new EditorScreen(m_LevelName, m_CameraPtr->GetPosition()));
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