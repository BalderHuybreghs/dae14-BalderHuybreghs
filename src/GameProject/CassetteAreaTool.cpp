#include "pch.h"
#include "CassetteAreaTool.h"

CassetteAreaTool::CassetteAreaTool(Level* levelPtr, const InputManager* inputManagerPtr)
  : RectangleTool("Cassette Area", inputManagerPtr), m_LevelPtr(levelPtr)
{
}

void CassetteAreaTool::OnPaintRect(const Rectf& rect)
{
  m_LevelPtr->SetCassetteZone(rect);
}
