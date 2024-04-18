#include "pch.h"
#include "BackgroundTool.h"

BackgroundTool::BackgroundTool(Level* levelPtr, const InputManager* inputManagerPtr)
  : RectangleTool("Background", inputManagerPtr), m_LevelPtr(levelPtr)
{
}

void BackgroundTool::OnPaintRect(const Rectf& rect)
{
}
