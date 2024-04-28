#include "pch.h"
#include "DeathZoneTool.h"

DeathZoneTool::DeathZoneTool(Level* levelPtr, const InputManager* inputManagerPtr)
  : RectangleTool("Death Zone", inputManagerPtr), m_LevelPtr(levelPtr)
{
}

void DeathZoneTool::Update(float elapsedSec, const Rectf& hoveringTile)
{
  RectangleTool::Update(elapsedSec, hoveringTile);

  // Remove a camera rect
  if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_RIGHT)) {
    m_LevelPtr->RemoveDeathZone(hoveringTile.Center());
  }
}

void DeathZoneTool::OnPaintRect(const Rectf& rect)
{
  m_LevelPtr->AddDeathZone(rect);
}
