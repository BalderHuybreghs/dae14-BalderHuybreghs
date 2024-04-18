#include "pch.h"
#include "RectangleTool.h"

RectangleTool::RectangleTool(const std::string& name, const InputManager* inputManagerPtr)
  : EditTool(name, inputManagerPtr), m_IsDrawing(false), m_StartPosition(Point2f()), m_EndPosition(Point2f())
{
}

void RectangleTool::Draw(const Camera* cameraPtr) const
{
  EditTool::Draw(cameraPtr);

  // Draw the current rect if the user is drawing
  if (m_IsDrawing) {
    cameraPtr->PushMatrix();
    utils::SetColor(Color4f{ 0.5f, 1.f, 1.f, 1.f }); // Random color lol
    utils::DrawRect(GetRect());
    cameraPtr->PopMatrix();
  }
}

void RectangleTool::Update(float elapsedSec, const Rectf& hoveringTile)
{
  if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_LEFT)) {
    if (m_IsDrawing) {
      const Point2f position{
        hoveringTile.left + hoveringTile.width,
        hoveringTile.bottom + hoveringTile.height
      };

      m_EndPosition = position;
      return;
    }

    const Point2f position{
      hoveringTile.left,
      hoveringTile.bottom
    };

    m_StartPosition = position;
    m_IsDrawing = true;
  } else {
    if (m_IsDrawing) {
      OnPaintRect(GetRect());
    }

    m_IsDrawing = false;
  }
}

Rectf RectangleTool::GetRect() const
{
  return Rectf(
    m_StartPosition.x,
    m_StartPosition.y,
    m_EndPosition.x - m_StartPosition.x,
    m_EndPosition.y - m_StartPosition.y
  );
}
