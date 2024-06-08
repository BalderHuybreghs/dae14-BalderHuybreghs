#include "pch.h"
#include "RectangleTool.h"

RectangleTool::RectangleTool(const std::string& name, const InputManager* inputManagerPtr)
  : EditTool(name, inputManagerPtr), m_IsDrawing(false), m_BottomLeft(Point2f()), m_TopRight(Point2f())
{
}

void RectangleTool::Draw(const Camera* cameraPtr) const
{
  EditTool::Draw(cameraPtr);

  // Draw the current rect if the user is drawing
  if (m_IsDrawing) {
    cameraPtr->PushMatrix();
    utils::SetColor(Color4f{ 0.5f, 0.5f, 0.5f, 1.f }); // Random color lol
    utils::DrawRect(GetRect());
    cameraPtr->PopMatrix();
  }
}

void RectangleTool::Update(float elapsedSec, const Rectf& hoveringTile)
{
  if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_LEFT)) {
    if (m_IsDrawing) {
      const Point2f position{
        hoveringTile.left,
        hoveringTile.bottom
      };

      // Take the biggest values as the end position and the smallest values as the start position
      m_TopRight = Point2f{
        std::max(position.x + hoveringTile.width, std::max(m_BottomLeft.x, m_TopRight.x)),
        std::max(position.y + hoveringTile.height, std::max(m_BottomLeft.y, m_TopRight.y))
      };

      m_BottomLeft = Point2f{
        std::min(position.x, std::min(m_BottomLeft.x, m_TopRight.x)),
        std::min(position.y, std::min(m_BottomLeft.y, m_TopRight.y))
      };

      return;
    }

    const Point2f position{
      hoveringTile.left,
      hoveringTile.bottom
    };

    m_TopRight = position;
    m_BottomLeft = position;
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
    m_BottomLeft.x,
    m_BottomLeft.y,
    m_TopRight.x - m_BottomLeft.x,
    m_TopRight.y - m_BottomLeft.y
  );
}
