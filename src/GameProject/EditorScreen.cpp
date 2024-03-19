#include "pch.h"
#include "EditorScreen.h"
#include "GameDefines.h"
#include <iostream>

EditorScreen::EditorScreen()
  : GameScreen(), m_LevelPtr(nullptr), m_CurrentTilemapPtr(nullptr), m_EditMode(Mode::TilesetFront), m_CurrentTile(0)
{
  
}

void EditorScreen::Initialize()
{
  m_LevelPtr = new Level(LEVEL1_NAME, "snow", TILEMAP_BG_PREFIX + "dirt");

  m_EditMode = Mode::TilesetFront;
  m_CurrentTilemapPtr = m_LevelPtr->GetFrontTilemap();
  m_CurrentTile = 0;

  //m_LevelPtr->Load();
}

EditorScreen::~EditorScreen()
{
  delete m_LevelPtr;
}

void EditorScreen::Draw()
{
  // Everything will be drawn with debug info in the editor by default
  m_LevelPtr->Draw(true);

  m_CurrentTilemapPtr->DrawSingleTile(Point2f{ 0, 0 }, m_CurrentTile);
}

void EditorScreen::Update(float elapsedSec)
{
  // The level itself should not be updated in the editor
}

void EditorScreen::OnKeyDownEvent(SDL_KeyboardEvent key)
{
  switch (key.keysym.sym)
  {
  case SDLK_RIGHT:
    m_CurrentTile = m_CurrentTile < m_CurrentTilemapPtr->GetTileCount() ? m_CurrentTile + 1 : m_CurrentTile;
    break;
  case SDLK_LEFT:
    m_CurrentTile = m_CurrentTile > 0 ? m_CurrentTile - 1 : 0;
    break;
  case SDLK_s:
    m_LevelPtr->Save();
  	break;
  }
}

void EditorScreen::OnKeyUpEvent(SDL_KeyboardEvent key)
{
}

void EditorScreen::OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
  const float mouseX{ float(e.x) };
  const float mouseY{ float(e.y) };

  if (e.state == SDL_BUTTON_LEFT) {
    m_CurrentTilemapPtr->SetTile(Point2f{ mouseX, mouseY }, m_CurrentTile);
  }
}

void EditorScreen::OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void EditorScreen::OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}
