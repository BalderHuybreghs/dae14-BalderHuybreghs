#include "pch.h"
#include "EditorScreen.h"
#include "GameDefines.h"

EditorScreen::EditorScreen()
  : GameScreen(), m_LevelPtr(nullptr)
{
  
}

void EditorScreen::Initialize()
{
  m_LevelPtr = new Level(LEVEL1_NAME, "snow", TILEMAP_BG_PREFIX + "dirt");
}

EditorScreen::~EditorScreen()
{
}

void EditorScreen::Draw()
{
  // Everything will be drawn with debug info in the editor by default
  m_LevelPtr->Draw(true);
}

void EditorScreen::Update(float elapsedSec)
{
  // The level itself should not be updated in the editor
}

void EditorScreen::OnKeyDownEvent(SDL_KeyboardEvent key)
{
}

void EditorScreen::OnKeyUpEvent(SDL_KeyboardEvent key)
{
}

void EditorScreen::OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
  if (e.state == SDL_MOUSEBUTTONDOWN) {

  }
}

void EditorScreen::OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void EditorScreen::OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}
