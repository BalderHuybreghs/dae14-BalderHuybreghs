#include "pch.h"
#include "BackgroundTool.h"

BackgroundTool::BackgroundTool(ParallaxBackground* backgroundPtr, const InputManager* inputManagerPtr)
  : RectangleTool("Background", inputManagerPtr), m_BackgroundPtr(backgroundPtr), m_Layer(BackgroundLayer::Back)
{
}

void BackgroundTool::OnMouseWheelEvent(const SDL_MouseWheelEvent& e)
{
  if (e.y < 0) // Scroll down
  {
    m_Layer = BackgroundLayer((int)m_Layer + 1 % 2);
  }
}

void BackgroundTool::OnPaintRect(const Rectf& rect)
{
  switch (m_Layer) {
    case BackgroundLayer::Back:
      m_BackgroundPtr->SetMidRect(rect);
      break;
    case BackgroundLayer::Front:
      m_BackgroundPtr->SetFrontRect(rect);
      break;
  }
}
