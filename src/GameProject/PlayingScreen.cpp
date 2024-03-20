#include "pch.h"
#include "PlayingScreen.h"
#include "GameDefines.h"

PlayingScreen::PlayingScreen()
  : m_LevelPtr(nullptr), m_PlayerPtr(nullptr), m_CameraPtr(nullptr)
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
  m_LevelPtr = new Level(LEVEL1_NAME, "snow", TILEMAP_BG_PREFIX + "dirt");

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

  // Collision stuffs
  if (m_TilemapPtr->IsTile(m_PlayerPtr->GetPosition())) {
    m_PlayerPtr->SetPosition(oldPlayerPosition);
  } else {
    m_PlayerPtr->ApplyForce(GRAVITY);
  }

  m_CameraPtr->SetPosition(m_PlayerPtr->GetPosition());
}

void PlayingScreen::OnKeyDownEvent(const SDL_KeyboardEvent& key)
{
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
