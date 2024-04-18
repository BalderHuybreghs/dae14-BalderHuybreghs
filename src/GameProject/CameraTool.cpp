#include "pch.h"
#include "CameraTool.h"
#include "utils.h"

CameraTool::CameraTool(Level* levelPtr, const InputManager* inputManagerPtr)
  : RectangleTool("Camera", inputManagerPtr), m_LevelPtr(levelPtr)
{
}

void CameraTool::Update(float elapsedSec, const Rectf& hoveringTile)
{
  RectangleTool::Update(elapsedSec, hoveringTile);

  // Remove a camera rect
  if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_RIGHT)) {
    m_LevelPtr->RemoveCameraRect(hoveringTile.Center());
  }
}

void CameraTool::OnPaintRect(const Rectf& rect)
{
  m_LevelPtr->AddCameraRect(rect);
}