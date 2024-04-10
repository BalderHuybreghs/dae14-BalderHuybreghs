#include "pch.h"
#include "TilemapTool.h"

TilemapTool::TilemapTool(Tilemap* tilemapPtr, const InputManager* inputManagerPtr)
  : EditTool("Tilemap", inputManagerPtr), m_TilemapPtr(tilemapPtr), m_CurrentTile(0)
{
}

void TilemapTool::Draw(const Camera* cameraPtr) const
{
  EditTool::Draw(cameraPtr);

  // Draw a preview tile
  const Point2f mousePos{ m_InputManagerPtr->GetMousePosition() };
  m_TilemapPtr->DrawSingleTile(Point2f{ mousePos.x + 10, mousePos.y - 60 }, m_CurrentTile);
}

void TilemapTool::Update(float elapsedSec, const Rectf& hoveringTile)
{
  if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_LEFT)) {
    m_TilemapPtr->SetTile(hoveringTile.Center(), m_CurrentTile);
  } else if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_RIGHT)) {
    m_TilemapPtr->RemoveTile(hoveringTile.Center());
  }
}

void TilemapTool::OnMouseWheelEvent(const SDL_MouseWheelEvent& e)
{
  if (e.y > 0) // Scroll up
  {
    m_CurrentTile = m_CurrentTile > 0 ? m_CurrentTile - 1 : 0;
  } else if (e.y < 0) // Scroll down
  {
    m_CurrentTile = m_CurrentTile < m_TilemapPtr->GetTileCount() - 1 ? m_CurrentTile + 1 : m_CurrentTile;
  }
}
